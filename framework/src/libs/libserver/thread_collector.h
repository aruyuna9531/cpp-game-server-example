#pragma once

#include "common.h"
#include "thread.h"
#include "cache_refresh.h"

class Packet;

// 集合了一类线程，需要将协议发送到所有线程协议。
// 和ThreadCollectorExclusive的区别是那个类处理互斥的情况（如MySQL线程），只要挑选一个线程处理。（见两边HandlerMessage的实现）
class ThreadCollector :public IDisposable
{
public:
    ThreadCollector(ThreadType threadType, int initNum);

    void CreateThread(int num);
    void DestroyThread();

    void Update();

    bool IsStopAll();
    bool IsDestroyAll();
    void Dispose() override;

    virtual void HandlerMessage(Packet* pPacket);
    virtual void HandlerCreateMessage(Packet* pPacket, bool isToAllThread);

protected:
    ThreadType _threadType;
    CacheRefresh<Thread> _threads;

    uint64 _nextThreadSn{ 0 };
};

