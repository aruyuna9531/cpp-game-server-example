#ifndef SVR_SUNNET_H_
#define SVR_SUNNET_H_

#include "service.h"
#include "worker.h"
#include<vector>
#include<memory>
#include<unordered_map>
#include<string>

class Worker;

class Sunnet {
private:
    Sunnet();
    Sunnet(const Sunnet &single) = delete;
    const Sunnet &operator=(const Sunnet &single) = delete;
public:
    static Sunnet* GetInst();

    void Start();
    void Wait();
private:
    int worker_num = 3;
    std::vector<Worker> workers;
    std::vector<std::thread> worker_threads;

    void StartWorker();

private:
    std::unordered_map<uint32_t, std::shared_ptr<Service>> services;    // 存放系统中的所有服务
    uint32_t maxServiceId = 0;
    pthread_rwlock_t servicesLock;
    std::shared_ptr<Service> GetService(uint32_t id);
public:
    uint32_t NewService(std::shared_ptr<std::string> type);
    void KillService(uint32_t id);

// 全局队列
private:
    std::queue<std::shared_ptr<Service>> globalQueue;
    int globalLen = 0;
    pthread_spinlock_t globalLock;
public:
    void Send(uint32_t toId, std::shared_ptr<BaseMsg> msg);
    void PushGlobalQueue(std::shared_ptr<Service> srv);
    std::shared_ptr<Service> PopGlobalQueue();

// 条件变量
private:
    pthread_mutex_t sleepMutex;
    pthread_cond_t sleepCond;
    int sleepCount = 0;
public:
    void CheckAndWakeUp();
    void WorkerWait();

// test
public:
    std::shared_ptr<BaseMsg> MakeMsg(uint32_t source, char* buff/*这个变量使用new*/, int len);
};

#endif // SUNNET_H_