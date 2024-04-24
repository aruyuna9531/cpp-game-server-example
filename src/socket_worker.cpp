#include "socket_worker.h"
#include "sunnet.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <signal.h>

void SocketWorker::Init() {
    std::cout << "socket worker init" << std::endl;
    epollfd = epoll_create(1024);
    assert(epollfd > 0);
}

void SocketWorker::operator()() {
    while(true) {
        const int event_size = 64;
        epoll_event events[event_size];
        int eventCount = epoll_wait(epollfd, events, event_size, -1);
        for (int i = 0; i < eventCount; i++) {
            epoll_event e = events[i];
            OnEvent(e);
        }
    }
}

// 添加事件
void SocketWorker::AddEvent(int fd) {
    std::cout << "add event fd " << fd << std::endl;
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;  // et边缘触发：一次没读完时也阻塞，下次wait响应后跟上次没读完的一起读（系统响应1次）。lt水平触发：一次没读完下次wait会一直读直到读完才阻塞（系统响应多次）
    ev.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        std::cout << "add event epoll_ctl fail: " << strerror(errno) << std::endl;
    }
}

void SocketWorker::RemoveEvent(int fd) {
    std::cout << "remove event fd " << fd << std::endl;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}

// epollout 需要时设定为true，给events添加EPOLLOUT监听（在write失败的时候添加，write成功了删除）
void SocketWorker::ModifyEvent(int fd, bool epollOut) {
    std::cout << "modify event fd " << fd << " " << epollOut << std::endl;
    epoll_event ev;
    ev.data.fd = fd;
    if (epollOut) {
        ev.events = EPOLLIN|EPOLLET|EPOLLOUT;
    } else {
        ev.events = EPOLLIN|EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void SocketWorker::OnEvent(epoll_event ev) {
    std::cout << "socker worker on event" << std::endl;
    int fd = ev.data.fd;
    auto conn = Sunnet::GetInst()->GetConn(fd);
    if (!conn) {
        std::cout << "OnEvent error, conn is null" << std::endl;
        return;
    }

    bool isRead = ev.events & EPOLLIN;
    bool isWrite = ev.events & EPOLLOUT;
    bool isError = ev.events & EPOLLERR;
    if (conn->type == Conn::TYPE::LISTEN) {
        if (isRead) {
            OnAccept(conn);
        }
    } else {
        // 普通socket
        if (isRead || isWrite) {
            OnRW(conn, isRead, isWrite);
        } else if (isError) {
            std::cout << "OnError fd " << conn->fd << std::endl;
        }
    }
}

void SocketWorker::OnAccept(std::shared_ptr<Conn> conn) {
    std::cout << "socker worker on accept" << std::endl;
    int clientfd = accept(conn->fd, NULL, NULL);
    if (clientfd < 0) {
        std::cout << "on accept error" << std::endl;
        return;
    }
    fcntl(clientfd, F_SETFL, O_NONBLOCK);
    Sunnet::GetInst()->AddConn(clientfd, conn->serviceId, Conn::TYPE::CLIENT);
    
    epoll_event ev;
    ev.events = EPOLLIN|EPOLLET;
    ev.data.fd = clientfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev) == -1) {
        std::cout << "on accept epoll_ctl fail: " << strerror(errno) << std::endl;
        return;
    }

    // 通知服务
    auto msg = std::make_shared<SocketAcceptMsg>();
    msg->type = BaseMsg::TYPE::SOCKET_ACCEPT;
    msg->listenfd = conn->fd;
    msg->clientfd = clientfd;
    Sunnet::GetInst()->Send(conn->serviceId, msg);
}

// 读写
void SocketWorker::OnRW(std::shared_ptr<Conn> conn, bool read, bool write) {
    std::cout << "socker worker on rw, fd = " << conn->fd << std::endl;
    auto msg = std::make_shared<SocketRWMsg>();
    msg->type = BaseMsg::TYPE::SOCKET_RW;
    msg->fd = conn->fd;
    msg->isRead = read;
    msg->isWrite = write;
    Sunnet::GetInst()->Send(conn->serviceId, msg);
}