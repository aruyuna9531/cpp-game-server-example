#include "worker.h"
#include "service.h"
#include "sunnet.h"
#include<iostream>
#include<unistd.h>

void Worker::operator()() {
    while(true) {
        std::shared_ptr<Service> srv = Sunnet::GetInst()->PopGlobalQueue();
        if (!srv) {
            // 线程休眠，等待唤醒
            std::cout << "worker " << this->id << " cannot get message, sleep" << std::endl;
            Sunnet::GetInst()->WorkerWait();
        } else {
            // 直接处理消息
            srv->ProcessMsgs(eachNum);
            CheckAndPutGlobal(srv); // 判断服务是否还有未处理消息，有的话放回全局队列。
        }
    }
}

void Worker::CheckAndPutGlobal(std::shared_ptr<Service> srv) {
    if (srv->isExiting) {
        return;
    }

    pthread_spin_lock(&srv->queueLock);
    if (!srv->msgQueue.empty()) {
        // srv is in global
        Sunnet::GetInst()->PushGlobalQueue(srv);
    } else {
        srv->SetInGlobal(false);
    }
    pthread_spin_unlock(&srv->queueLock);
}