#ifndef SVR_WORKER_H_
#define SVR_WORKER_H_
#include<thread>
#include<memory>
class Sunnet;
class Service;

// 每条线程一个worker
class Worker {
public:
    int id;
    int eachNum;        // 每次处理消息数
    void operator()();  // 线程函数
    void CheckAndPutGlobal(std::shared_ptr<Service> srv);
};
#endif