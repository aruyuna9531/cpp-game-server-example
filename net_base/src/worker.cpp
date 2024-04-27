#include "worker.h"
#include "service.h"
#include "sunnet.h"
#include<iostream>
#include<unistd.h>
#include<signal.h>

Worker::~Worker() {
    std::cout << "worker " << id << " closed" << std::endl;
}
void Worker::operator()(std::shared_ptr<std::promise<int>>&& exitP) {
    while(Sunnet::GetInst()->running) {
        std::shared_ptr<Service> srv = Sunnet::GetInst()->PopGlobalQueue();
        if (!srv) {
            // 线程休眠，等待唤醒
            std::cout << "worker " << this->id << " cannot get message, sleep" << std::endl;
            Sunnet::GetInst()->WorkerWait();        // worker等消息（cv阻塞）
        } else {
            // 直接处理消息
            srv->ProcessMsgs(eachNum);
            CheckAndPutGlobal(srv); // 判断服务是否还有未处理消息，有的话保持service在全局队列
        }
    }
    std::cout << "worker " << id << " finish" << std::endl;
    exitP->set_value_at_thread_exit(id); // 在正常退出时分发这个值
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