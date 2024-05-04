#include "thread_collector_exclusive.h"
#include "packet.h"
#include "message_system.h"

ThreadCollectorExclusive::ThreadCollectorExclusive(ThreadType threadType, int initNum) :ThreadCollector(threadType, initNum)
{
}

void ThreadCollectorExclusive::HandlerMessage(Packet* pPacket)
{
    if (pPacket->GetMsgId() == Proto::MsgId::MI_CmdThread)
    {
        // 显示指令，每一个线程都执行
        HandlerCreateMessage(pPacket, true);
    }
    else
    {
        // 并不是每一个线程都要执行，只挑一个执行就行（_nextThreadSn）
        auto objs = _threads.GetReaderCache();
        auto iter = objs->find(_nextThreadSn);
        if (iter == objs->end()) {
            iter = objs->begin();
        }

        iter->second->GetMessageSystem()->AddPacketToList(pPacket);
        iter++;
        if (iter == objs->end()) {
            iter = objs->begin();
        }

        _nextThreadSn = iter->first;
    }
}
