#include "service.h"
#include "sunnet.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <signal.h>

Service::Service() {
    pthread_spin_init(&queueLock, PTHREAD_PROCESS_PRIVATE);
    pthread_spin_init(&inGlobalLock, PTHREAD_PROCESS_PRIVATE);
}

Service::~Service() {
    std::cout << "service " << this->id << " finished" << std::endl;
    pthread_spin_destroy(&queueLock);
    pthread_spin_destroy(&inGlobalLock);
}

void Service::PushMsg(std::shared_ptr<BaseMsg> msg) {
    pthread_spin_lock(&queueLock);
    msgQueue.push(msg);
    pthread_spin_unlock(&queueLock);
}

std::shared_ptr<BaseMsg> Service::PopMsg() {
    std::shared_ptr<BaseMsg> msg = nullptr;
    pthread_spin_lock(&queueLock);
    if (!msgQueue.empty()) {
        msg = msgQueue.front();
        msgQueue.pop();
    }
    pthread_spin_unlock(&queueLock);
    return msg;
}

void Service::OnInit() {
    std::cout << "[" << id << "] OnInit" << std::endl;
    listenfd = Sunnet::GetInst()->Listen(8002, id);
    std::cout << "[" << id << "] listening at port 8002" << std::endl;

    std::cout << "[" << id << "] creating lua vm" << std::endl;
    luaState = luaL_newstate();
    luaL_openlibs(luaState);
    std::string filename = "../service/" + *type + "/init.lua";
    int ok = luaL_dofile(luaState, filename.data());
    if (ok == 1) {
        std::cout << "run lua fail: " << lua_tostring(luaState, -1) << std::endl;
    }
}

void Service::OnMsg(std::shared_ptr<BaseMsg> msg) {
    // test 
    switch (msg->type) {
        case BaseMsg::TYPE::SERVICE:
        {
            std::shared_ptr<ServiceMsg> m = std::dynamic_pointer_cast<ServiceMsg>(msg);
            OnServiceMsg(m);
            break;
        }
        case BaseMsg::TYPE::SOCKET_ACCEPT:
        {
            std::shared_ptr<SocketAcceptMsg> m2 = std::dynamic_pointer_cast<SocketAcceptMsg>(msg);
            OnAcceptMsg(m2);
            break;
        }
        case BaseMsg::TYPE::SOCKET_RW:
        {
            auto m3 = std::dynamic_pointer_cast<SocketRWMsg>(msg);
            OnRWMsg(m3);
            break;
        }
    }
}

void Service::OnExit() {
    std::cout << "[" << id << "] OnExit" << std::endl;
    lua_close(luaState);
}

bool Service::ProcessMsg() {
    std::shared_ptr<BaseMsg> msg = PopMsg();
    if (msg) {
        OnMsg(msg);
        return true;
    } else {
        return false;
    }
}

void Service::ProcessMsgs(int max) {
    for (int i = 0; i < max; i++) {
        if (!ProcessMsg()) {
            break;
        }
    }
}

void Service::SetInGlobal(bool is) {
    pthread_spin_lock(&inGlobalLock);
    inGlobal = is;
    pthread_spin_unlock(&inGlobalLock);
}

void Service::OnServiceMsg(std::shared_ptr<ServiceMsg> msg) {
    std::cout << "[" << id << "] OnServiceMsg, data = " << msg->buff << std::endl;
    std::shared_ptr<BaseMsg> msgRet = Sunnet::GetInst()->MakeMsg(id, new char[10]{'p','i','n','g','\0'}, 10);    // 7个9是检查内存泄漏的
    Sunnet::GetInst()->Send(msg->source, msgRet);
}

void Service::OnAcceptMsg(std::shared_ptr<SocketAcceptMsg> msg) {
    std::cout << "[" << id << "] OnAcceptMsg, fd = " << msg->clientfd << std::endl;
    auto w = std::make_shared<ConnWriter>();
    w->fd = msg->clientfd;
    writers.emplace(msg->clientfd, w);  // 注册writer
}

void Service::OnRWMsg(std::shared_ptr<SocketRWMsg> msg) {
    int fd = msg->fd;
    if (msg->isRead) {
        const int buffsize = 512;
        char buff[buffsize];
        int len = 0;
        do {
            len = read(fd, &buff, buffsize);
            if (len > 0) {
                OnSocketData(fd, buff, len);    // 可读，read成功（客户端发来消息）
            }
        } while(len == buffsize);
        if (len <= 0 && errno != EAGAIN) {  // EAGAIN表示数据读完正常退出，buff要读的刚好是buffsize倍数时会发生，这里要屏蔽这种情况
            // 可读，read失败（客户端关闭）
            // if的原因：如果要处理多条消息，在处理到中途因故主动断开，，处理后续的消息时也会走到这里，这部分函数会执行2次，socket close就有2次。加上判断保证socket close只执行一次。
            if (Sunnet::GetInst()->GetConn(fd)) {
                OnSocketClose(fd);
                Sunnet::GetInst()->CloseConn(fd);
            }
        }
    }
    if (msg->isWrite) {
        if (Sunnet::GetInst()->GetConn(fd)) {
            OnSocketWritable(fd);
        }
    }
}
void Service::OnSocketData(int fd, const char* buff, int len) {
    std::cout << "OnSocketData fd = " << fd << " buff: " << buff << std::endl;
    // char* writeBuff = new char[4200000]{0};      // 这里一定是new的，否则会double free宕机，下面要被shared_ptr包起来，回忆下智能指针包裸指针的问题
    // writeBuff[4199998] = 'e';
    // int r = write(fd, &writeBuff, sizeof(writeBuff));
    // std::cout << "write r: " << r << " " << strerror(errno) << std::endl;
    // auto w = writers[fd];
    // w->EntireWrite(std::shared_ptr<char>(writeBuff), 4200000);
    // w->LingerClose();
}
void Service::OnSocketWritable(int fd) {
    auto w = writers[fd];
    if (w) {
        w->OnWriteable();
    }
}

void Service::OnSocketClose(int fd) {
    writers.erase(fd);
}