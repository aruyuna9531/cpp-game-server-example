#include "sunnet.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <thread>
Sunnet::Sunnet() {
}
Sunnet::~Sunnet() {
    std::cout << "sunnet destructing" << std::endl;

    sleepCount = 0;
    pthread_cond_broadcast(&sleepCond);         // 这里是唤醒所有工作线程让它们退出，否则会卡在wait cv那里
    for (auto iter = netListeningFds.begin(); iter != netListeningFds.end(); iter++) {
        bool succ = RemoveConn(*iter);
        close(*iter);
        if (succ) {
            socketWorker.RemoveEvent(*iter);
        }
    }

    pthread_rwlock_destroy(&servicesLock);
    pthread_spin_destroy(&globalLock);
    pthread_mutex_destroy(&sleepMutex);
    pthread_cond_destroy(&sleepCond);
    pthread_rwlock_destroy(&connsLock);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Sunnet::Start() {
    // 处理亿些信号
    signal(SIGPIPE, SIG_IGN);       // 屏蔽SIGPIPE信号（对端崩溃己端无法立即反应还会正常发消息，对端收到异常消息会返回RST，对RST套接字write会导致收到来自操作系统的SIGPIPE，会杀掉进程）。有优雅处理方式，但现在先偷个懒

    pthread_rwlock_init(&servicesLock, NULL);   // 初始化服务锁
    pthread_spin_init(&globalLock, PTHREAD_PROCESS_PRIVATE);    // 全局队列锁
    pthread_mutex_init(&sleepMutex, NULL);
    pthread_cond_init(&sleepCond, NULL);
    pthread_rwlock_init(&connsLock, NULL);
    
    // start worker
    for (int i = 0; i < worker_num; i++) {
        std::cout << "start worker thread: " << i << std::endl;
        Worker worker;
        worker.id = i;
        worker.eachNum = (2 << i);
        workers.push_back(worker);
        std::shared_ptr<std::promise<int>> wp = std::make_shared<std::promise<int>>();
        worker_promises.push_back(wp->get_future());
        worker_threads.push_back(std::thread(worker, wp));
    }
    
    // start socket
    socketWorker.Init();
    std::shared_ptr<std::promise<int>> sp = std::make_shared<std::promise<int>>();
    socketFuture = sp->get_future();
    socketThread = std::move(std::thread(socketWorker, std::move(sp)));


}
void Sunnet::Wait() {
    for (int i = 0; i < this->worker_threads.size(); i++) {
        worker_threads[i].detach();
    }
    socketThread.detach();

    for (int i = 0; i < this->worker_promises.size(); i++) {
        worker_promises[i].wait();
        std::cout << "sunnet: worker " << worker_promises[i].get() << " << finished" << std::endl;
    }
    socketFuture.wait();
    std::cout << "sunnet: socket worker " << socketFuture.get() << " << finished" << std::endl;
}

Sunnet* Sunnet::GetInst() {
    static Sunnet inst;
    return &inst;
}

// 新建服务
uint32_t Sunnet::NewService(std::shared_ptr<std::string> type) {
    std::shared_ptr<Service> srv = std::make_shared<Service>();
    srv->type = type;
    pthread_rwlock_wrlock(&servicesLock);   // 写
    srv->id = maxServiceId;
    maxServiceId++;
    services.emplace(srv->id, srv);
    pthread_rwlock_unlock(&servicesLock);
    srv->OnInit();
    return srv->id;
}

// 查找服务
std::shared_ptr<Service> Sunnet::GetService(uint32_t id) {
    std::shared_ptr<Service> srv = nullptr;
    pthread_rwlock_rdlock(&servicesLock);   // 读
    auto iter = services.find(id);
    if (iter != services.end()) {
        srv = iter->second;
    }
    pthread_rwlock_unlock(&servicesLock);
    return srv;
}

// 删除服务
void Sunnet::KillService(uint32_t id) {
    std::shared_ptr<Service> srv = GetService(id);
    if (!srv) {
        return;
    }
    srv->OnExit();
    srv->isExiting = true;  // 这里先设值 因为不排除其他服务仍然在调用这个要kill掉的srv
    pthread_rwlock_wrlock(&servicesLock);
    services.erase(id);
    pthread_rwlock_unlock(&servicesLock);
}

// 弹出全局队列
std::shared_ptr<Service> Sunnet::PopGlobalQueue() {
    std::shared_ptr<Service> srv = NULL;
    pthread_spin_lock(&globalLock);
    if (!globalQueue.empty()) {
        srv = globalQueue.front();
        globalQueue.pop();
        globalLen--;
    }
    pthread_spin_unlock(&globalLock);
    return srv;
}
// 插入全局队列
void Sunnet::PushGlobalQueue(std::shared_ptr<Service> srv) {
    pthread_spin_lock(&globalLock);
    globalQueue.push(srv);
    globalLen++;
    pthread_spin_unlock(&globalLock);
}

void Sunnet::Send(uint32_t toId, std::shared_ptr<BaseMsg> msg) {
    std::shared_ptr<Service> toSrv = GetService(toId);
    if (!toSrv) {
        std::cout << "send error, id " << toId << " service not exist" << std::endl;
        return;
    }

    toSrv->PushMsg(msg);
    bool hasPush = false;
    pthread_spin_lock(&toSrv->inGlobalLock);
    if (!toSrv->inGlobal) {
        PushGlobalQueue(toSrv);
        toSrv->inGlobal = true;
        hasPush = true;
    }
    pthread_spin_unlock(&toSrv->inGlobalLock);
    if (hasPush) {
        // 新消息来了，叫醒线程处理
        CheckAndWakeUp();
    }
}

// test
std::shared_ptr<BaseMsg> Sunnet::MakeMsg(uint32_t source, char* buff/*这个变量使用new*/, int len) {
    std::shared_ptr<ServiceMsg> msg = std::make_shared<ServiceMsg>();
    msg->type = BaseMsg::TYPE::SERVICE;
    msg->source = source;
    msg->buff = std::shared_ptr<char>(buff);
    msg->size = len;
    return msg;
}

// 休眠并等待条件
void Sunnet::WorkerWait() {
    pthread_mutex_lock(&sleepMutex);
    sleepCount++;
    pthread_cond_wait(&sleepCond, &sleepMutex);
    sleepCount--;
    pthread_mutex_unlock(&sleepMutex);
}
// 检查并唤醒线程
void Sunnet::CheckAndWakeUp() {
    // 这里sleepCount不加锁，值可能是不准确的，但异常情况的开销是比加锁开销小的，可以接受
    // （1）sleepCount小了，该唤醒时没唤醒，只需要等待下一次
    // （2）sleepCount大了，就当工作线程空转一次
    if (sleepCount == 0) {
        return;
    }
    if (worker_num - sleepCount <= globalLen) {
        std::cout << "wake up a service" << std::endl;
        pthread_cond_signal(&sleepCond);
    }
}

// 添加连接
int Sunnet::AddConn(int fd, uint32_t id, Conn::TYPE type) {
    std::shared_ptr<Conn> conn = std::make_shared<Conn>();
    conn->fd = fd;
    conn->serviceId = id;
    conn->type = type;
    pthread_rwlock_wrlock(&connsLock);
    conns.emplace(fd, conn);
    pthread_rwlock_unlock(&connsLock);
    return fd;
}

// 获得连接
std::shared_ptr<Conn> Sunnet::GetConn(int fd) {
    std::shared_ptr<Conn> conn = nullptr;
    pthread_rwlock_rdlock(&connsLock);
    auto iter = conns.find(fd);
    if (iter != conns.end()) {
        conn = iter->second;
    }
    pthread_rwlock_unlock(&connsLock);
    return conn;
}

// 删除连接
bool Sunnet::RemoveConn(int fd) {
    int result = 0;
    pthread_rwlock_wrlock(&connsLock);
    result = conns.erase(fd);
    pthread_rwlock_unlock(&connsLock);
    return result == 1;
}

int Sunnet::Listen(uint32_t port, uint32_t serviceId) {
    // 创建socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        std::cout << "listen error, socket fail" << std::endl;
        return -1;
    };
    fcntl(listenfd, F_SETFL, O_NONBLOCK);   // fd默认阻塞，要设置成非阻塞

    // bind
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int r = bind(listenfd, (sockaddr*)&addr, sizeof(addr));
    if (r == -1) {
        std::cout << "listen error, bind fail" << std::endl;
        return -1;
    }
    // 监听
    r = listen(listenfd, 64);
    if (r < 0) {
        std::cout << "listen error, listen fail" << std::endl;
        return -1;
    }
    AddConn(listenfd, serviceId, Conn::TYPE::LISTEN);   // Listen的线程安全性：AddConn有读写锁
    socketWorker.AddEvent(listenfd);   // Listen的线程安全性：内部epoll_ctl由操作系统保证系统安全性
    netListeningFds.insert(listenfd);
    return listenfd;
}

void Sunnet::ModifyEvent(int fd, bool epollOut) {
    socketWorker.ModifyEvent(fd, epollOut);
}

void Sunnet::CloseConn(uint32_t fd) {
    bool succ = RemoveConn(fd);
    close(fd);
    if (succ) {
        socketWorker.RemoveEvent(fd);
        netListeningFds.erase(fd);
    }
}