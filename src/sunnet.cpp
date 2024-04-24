#include "sunnet.h"
#include <iostream>

Sunnet::Sunnet() {
}

void Sunnet::Start() {
    std::cout << "Hello Sunnet" << std::endl;
    pthread_rwlock_init(&servicesLock, NULL);   // 初始化服务锁
    pthread_spin_init(&globalLock, PTHREAD_PROCESS_PRIVATE);    // 全局队列锁
    pthread_mutex_init(&sleepMutex, NULL);
    pthread_cond_init(&sleepCond, NULL);
    // TODO 销毁锁
    StartWorker();
}
void Sunnet::Wait() {
    for (int i = 0; i < this->worker_threads.size(); i++) {
        worker_threads[i].join();
    }
}

Sunnet* Sunnet::GetInst() {
    static Sunnet inst;
    return &inst;
}

void Sunnet::StartWorker() {
    for (int i = 0; i < worker_num; i++) {
        std::cout << "start worker thread: " << i << std::endl;
        Worker worker;
        worker.id = i;
        worker.eachNum = (2 << i);
        workers.push_back(worker);
        worker_threads.push_back(std::thread(worker));
    }
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