#pragma once
#include "thread_collector.h"

// 集合了一类互斥线程，只要挑选一个线程处理（如MySQL线程）。
// 和ThreadCollector的区别是那个类不处理互斥的情况（如逻辑线程），。（见两边HandlerMessage的实现）
class ThreadCollectorExclusive :public ThreadCollector
{
public:
    ThreadCollectorExclusive(ThreadType threadType, int initNum);

    virtual void HandlerMessage(Packet* pPacket) override;
};

