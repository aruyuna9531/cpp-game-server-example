#ifndef SVR_MSG_H_
#define SVR_MSG_H_
#include<memory>
class BaseMsg {
public:
    enum TYPE {
        SERVICE = 1,        // 服务消息
        SOCKET_ACCEPT = 2,  // 新的客户端连接
        SOCKET_RW = 3,      // 客户端读写消息
    };
    TYPE type;
    char load[10000]{};
    virtual ~BaseMsg(){};
};

// 服务器消息
class ServiceMsg: public BaseMsg {
public:
    uint32_t source;
    size_t size;
    std::shared_ptr<char> buff;
};

// 新连接
class SocketAcceptMsg: public BaseMsg {
public:
    int listenfd;   // 监听套接字fd
    int clientfd;   // 客户端套接字fd
};

// socket读写
class SocketRWMsg: public BaseMsg {
public:
    int fd;
    bool isRead = false;
    bool isWrite = false;
};
#endif