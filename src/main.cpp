#include "sunnet.h"
#include "worker.h"
#include <unistd.h>
#include <string>

int main() {
    // daemon(0,0);     // 开启守护进程，但标准输出会被弃掉
    Sunnet::GetInst()->Start();

    std::shared_ptr<std::string> pingType = std::make_shared<std::string>("ping");
    uint32_t ping1 = Sunnet::GetInst()->NewService(pingType);
    uint32_t ping2 = Sunnet::GetInst()->NewService(pingType);
    uint32_t pong = Sunnet::GetInst()->NewService(pingType);

    auto msg1 = Sunnet::GetInst()->MakeMsg(ping1, new char[3]{'a','?','\0'}, 3);
    auto msg2 = Sunnet::GetInst()->MakeMsg(ping2, new char[3]{'O','K','\0'}, 3);
    Sunnet::GetInst()->Send(pong, msg1);
    Sunnet::GetInst()->Send(pong, msg2);

    Sunnet::GetInst()->Wait();
    return 0;
}