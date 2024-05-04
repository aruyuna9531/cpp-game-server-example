#pragma once
#include "util_time.h"
#include "system.h"
#include "entity.h"

#include <list>
#include <vector>

struct Timer
{
    timeutil::Time NextTime;    // 下次调用时间
    TimerHandleFunction Handler;// 调用函数
    int DelaySecond;            // 首次执行时延迟秒
    int DurationSecond;         // 间隔时间(秒）
    int CallCountTotal;         // 总调用次数（0为无限）
    int CallCountCur;           // 当前调用次数 
    uint64 SN;                  // 方便删除数据时找到Timer
};

// 对一个线程，只需要一个TimerComponent
class TimerComponent :public Entity<TimerComponent>, public IAwakeSystem<>
{
public:
    void Awake() override;          // 初始化
    void BackToPool() override;     

    uint64 Add(int total, int durations, bool immediateDo, int immediateDoDelaySecond, TimerHandleFunction handler);
    void Remove(std::list<uint64>& timers);

    bool CheckTime();               // 检查是否有需要执行的节点
    Timer PopTimeHeap();

    void Update();                  // 更新

protected:
    void Add(Timer& data);

private:
    std::vector<Timer> _heap;       // 最小时间堆，用std::make_heap、std::push_heap、std::pop_heap操作
};