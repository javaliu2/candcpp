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
    void setReuseAddr(bool enable = true);
    void setReusePort(bool enable = true);

    void close();
    // 禁止拷贝
    BaseSocket(const BaseSocket&) = delete;
    BaseSocket& operator=(const BaseSocket&) = delete;
};