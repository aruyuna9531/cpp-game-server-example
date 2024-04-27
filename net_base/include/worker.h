#ifndef SVR_WORKER_H_
#define SVR_WORKER_H_
#include<thread>
#include<memory>
#include<future>
class Sunnet;
class Service;

// 每条线程一个worker。管理service，有事起来处理
class Worker {
public:
    int id;
    int eachNum;        // 每次处理消息数
    ~Worker();
    void operator()(std::shared_ptr<std::promise<int>>&& exitP);  // 线程函数
    void CheckAndPutGlobal(std::shared_ptr<Service> srv);
};
#endif