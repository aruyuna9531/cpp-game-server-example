#include "service.h"
#include "sunnet.h"
#include <iostream>

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
}

void Service::OnMsg(std::shared_ptr<BaseMsg> msg) {
    // test 
    if (msg->type == BaseMsg::TYPE::SERVICE) {
        std::shared_ptr<ServiceMsg> m = std::dynamic_pointer_cast<ServiceMsg>(msg);
        std::cout << "[" << id << "] OnMsg, data = " << m->buff << std::endl;
        auto msgRet = Sunnet::GetInst()->MakeMsg(id, new char[10]{'p','i','n','g','\0'}, 10);    // 7个9是检查内存泄漏的
        Sunnet::GetInst()->Send(m->source, msgRet);
    } else {
        std::cout << "[" << id << "] OnMsg called, but do nothing" << std::endl;
    }
}

void Service::OnExit() {
    std::cout << "[" << id << "] OnExit" << std::endl;
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