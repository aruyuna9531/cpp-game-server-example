#ifndef SVR_CONN_H_
#define SVR_CONN_H_

// 连接。与套接字一一对应
class Conn {
public:
    enum TYPE {
        LISTEN = 1, // 监听连接
        CLIENT = 2, // 客户端连接
    };
    uint8_t type;
    int fd;     // socket fd
    uint32_t serviceId; // 服务id
};
#endif