#include "BaseSocket.h"
#include <iostream>

BaseSocket::BaseSocket() {
    // fd == ::socket(AF_INET, SOCK_STREAM, 0); // bug error, this is comparator operator
    // 相当于进行比较，而忽略比较的结果
    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("BaseSocket::BaseSocket() create socket error");
        throw std::runtime_error("BaseSocket::BaseSocket() create socket failed");
    }
}

BaseSocket::BaseSocket(int fd) : fd(fd) {

}

BaseSocket::~BaseSocket() {
    if (fd != -1) {
        ::close(fd);
        std::cout << "成功关闭文件描述符:" << fd << std::endl;
    }
}

void BaseSocket::setNonBlocking(bool enable) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl get error");
        return;
    }
    if (enable) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl set error");
    }
}

void BaseSocket::setReuseAddr(bool enable) {
    int opt = enable ? 1 : 0;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt SO_REUSEADDR error");
    }
}

void BaseSocket::setReusePort(bool enable) {
    int opt = enable ? 1 : 0;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        perror("setsockopt SO_REUSEPORT error");
    }
}

void BaseSocket::close() {
    if (fd != -1) {
        ::close(fd);
        fd = -1;
    }
}