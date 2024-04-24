#ifndef SVR_SERVICE_H_
#define SVR_SERVICE_H_

#include<queue>
#include<thread>
#include<memory>
#include "msg.h"

class Service {
public:
    uint32_t id;
    std::shared_ptr<std::string> type;
    bool isExiting = false;
    std::queue<std::shared_ptr<BaseMsg>> msgQueue;
    pthread_spinlock_t queueLock;   // 自旋锁，循环检查锁是否释放。检查期间线程是一直running的，互斥锁在阻塞期间是sleep。临界区较小时开销比互斥锁小，性能比互斥锁好（因为互斥锁有一个唤醒过程开销大）。但由于自旋锁保持running，只适用于多核服务器（并且长期有核资源给它自旋）的场合。
public:
    Service();
    ~Service();
    void OnInit();
    void OnMsg(std::shared_ptr<BaseMsg> msg);
    void OnExit();
    void PushMsg(std::shared_ptr<BaseMsg> msg);
    bool ProcessMsg();
    void ProcessMsgs(int max);
private:
    std::shared_ptr<BaseMsg> PopMsg();

public:
    bool inGlobal = false;
    pthread_spinlock_t inGlobalLock;
    void SetInGlobal(bool is);
};
#endif