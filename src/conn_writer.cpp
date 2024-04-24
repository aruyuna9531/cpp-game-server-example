#include "conn_writer.h"
#include "sunnet.h"
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>

void ConnWriter::EntireWriteWhenEmpty(std::shared_ptr<char> buff, std::streamsize len) {
    char* s = buff.get() ;
    std::streamsize n = write(fd, s, len);
    if(n < 0 && errno == EINTR) {
        std::cout << "EntireWriteWhenEmpty error: interrupted" << std::endl;
        return;
    }
    std::cout << "EntireWriteWhenEmpty write n = " << n << std::endl;
    if(n >= 0 && n == len) {
        // 写完
        std::cout << "EntireWriteWhenEmpty write over" << std::endl;
        return;
    }
    if( (n > 0 && n < len) || (n < 0 && errno == EAGAIN) ) {
        // 写了亿点，推进去等下次
        auto obj = std::make_shared<WriteObject>();
        obj->start = n;
        obj->buff = buff;
        obj->len = len;
        objs.push_back(obj);
        Sunnet::GetInst()->ModifyEvent(fd, true);
        return;
    }
    // 出错
    std::cout << "EntireWrite write error: " << strerror(errno) << std::endl;
}

void ConnWriter::EntireWriteWhenNotEmpty(std::shared_ptr<char> buff, std::streamsize len) {
    // 还有人等着写，先推
    auto obj = std::make_shared<WriteObject>();
    obj->start = 0;
    obj->buff = buff;
    obj->len = len;
    objs.push_back(obj);
}

void ConnWriter::EntireWrite(std::shared_ptr<char> buff, std::streamsize len) {
    if (isClosing) {
        std::cout << "EntireWrite fail, isClosing" << std::endl;
        return;
    }
    if (objs.empty()) {
        EntireWriteWhenEmpty(buff, len);
    } else {
        EntireWriteWhenNotEmpty(buff, len);
    }
}


// true 写完一条
bool ConnWriter::WriteFrontObj() {
    if(objs.empty()) {
        return false;
    }
    auto obj = objs.front();

    char* s = obj->buff.get() + obj->start;
    int len = obj->len - obj->start;
    int n = write(fd, s, len);
    std::cout << "WriteFrontObj write n = " << n << std::endl;
    if(n < 0 && errno == EINTR) {
        std::cout << "WriteFrontObj error: interrupted" << std::endl;
        return false;
    }
    if(n >= 0 && n == len) {
        // 写完
        std::cout << "WriteFrontObj write over, pop front" << std::endl;
        objs.pop_front();
        return true;
    }
    if( (n > 0 && n < len) || (n < 0 && errno == EAGAIN) ) {
        // 写了亿点
        obj->start += n;
        return false;
    }
    // 报错
    std::cout << "EntireWrite write error " << std::endl;
    return false;
}

void ConnWriter::OnWriteable() {
    auto conn = Sunnet::GetInst()->GetConn(fd);
    if(conn == NULL){
        return;
    }

    while(WriteFrontObj());
    
    if(objs.empty()) {
        Sunnet::GetInst()->ModifyEvent(fd, false);   // 写成功，取消可写监听
        if(isClosing) {
            //通知服务，此处并不是通用做法
            //让read产生 Bad file descriptor报错
            std::cout << "linger close conn" << std::endl;
            shutdown(fd, SHUT_RD);
            auto msg= std::make_shared<SocketRWMsg>();
            msg->type = BaseMsg::TYPE::SOCKET_RW;
            msg->fd = conn->fd;
            msg->isRead = true;
            Sunnet::GetInst()->Send(conn->serviceId, msg);
        }
    }
}

void ConnWriter::LingerClose(){
    if(isClosing){
        return;
    }
    isClosing = true;
    if(objs.empty()) {
        Sunnet::GetInst()->CloseConn(fd);
        return;
    }
}

WriteObject::~WriteObject() {
    std::cout << "Write Object addr " << this << " destructing" << std::endl;
}