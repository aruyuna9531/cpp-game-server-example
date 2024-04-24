#ifndef SVR_MSG_H_
#define SVR_MSG_H_
#include<memory>
class BaseMsg {
public:
    enum TYPE {
        SERVICE = 1,        // 服务消息
        SOCKET_ACCEPT = 2;  // 新的客户端连接
        SOCKET_RW = 3;      // 客户端读写消息
    };
    uint8_t type;
    char load[10000]{};
    virtual ~BaseMsg(){};
};

class ServiceMsg: public BaseMsg {
public:
    uint32_t source;
    size_t size;
    std::shared_ptr<char> buff;
};
#endif