#ifndef SVR_SOCKET_WORKER_H_
#define SVR_SOCKET_WORKER_H_

#include <sys/epoll.h>
#include <memory>
#include "conn.h"

class SocketWorker {
private:
    int epollfd;        // epoll描述符

public:
    void Init();
    void operator()();

    void AddEvent(int fd);
    void RemoveEvent(int fd);
    void ModifyEvent(int fd, bool epollOut);
private:
    void OnEvent(epoll_event ev);
    void OnAccept(std::shared_ptr<Conn> conn);
    void OnRW(std::shared_ptr<Conn> conn, bool read, bool write);
};
#endif