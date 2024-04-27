#ifndef SVR_SOCKET_WORKER_H_
#define SVR_SOCKET_WORKER_H_

#include <sys/epoll.h>
#include <memory>
#include <future>
#include "conn.h"

class SocketWorker {
private:
    int epollfd;        // epoll描述符
public:
    ~SocketWorker();
    void Init();
    void operator()(std::shared_ptr<std::promise<int>>&& exitP);
    int getEpollFd();

    void AddEvent(int fd);
    void RemoveEvent(int fd);
    void ModifyEvent(int fd, bool epollOut);
private:
    void OnEvent(epoll_event ev);
    void OnAccept(std::shared_ptr<Conn> conn);
    void OnRW(std::shared_ptr<Conn> conn, bool read, bool write);
};
#endif