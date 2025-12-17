#pragma once
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>
#include "InetAddress.h"

class BaseSocket {
protected:
    int fd;
public:
    BaseSocket();
    explicit BaseSocket(int fd);
    virtual ~BaseSocket();

    int getFd() const { return fd; };
    void setNonBlocking(bool enable = true);
    void setReuseAddr(bool enable = true);  // 允许服务器端socket绑定处于TIME_WAIT状态的TCP连接占有的地址(ip:port)
    void setReusePort(bool enable = true);  // 允许多个进程(socket)绑定、监听同一个地址

    void close();
    // 禁止拷贝
    BaseSocket(const BaseSocket&) = delete;
    BaseSocket& operator=(const BaseSocket&) = delete;
};