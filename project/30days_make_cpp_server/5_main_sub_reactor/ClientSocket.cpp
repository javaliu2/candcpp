#include "ClientSocket.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>

ClientSocket::ClientSocket() : BaseSocket(), state(State::Created) {}

ClientSocket::ClientSocket(int fd, const InetAddress& addr_) 
    : BaseSocket(fd), addr(addr_), state(State::Created) {
}

ClientSocket::ClientSocket(const InetAddress& addr_) 
    : BaseSocket(), addr(addr_), state(State::Created) {
    // 如果构造时有地址，可以直接尝试 connect
    connect();
}

void ClientSocket::setAddress(const InetAddress& addr_) {
    if (state == State::Connected) {
        throw std::runtime_error("Cannot change address after connected");
    }
    addr = addr_;
}

bool ClientSocket::connect() {
    if (state == State::Connected) {
        std::cerr << "Socket already connected\n";
        return false;
    }
    
    if (::connect(fd, addr.getSockAddr(), addr.getSockLen()) == -1) {
        perror("connect error");
        if (errno != EINPROGRESS) {
            return false;
        }
    }

    state = State::Connected;
    return true;
}

ssize_t ClientSocket::read(char* buf, size_t len) {
    if (state != State::Connected) {
        throw std::runtime_error("Socket not connected, cannot read");
    }
    ssize_t n = ::read(fd, buf, len);
    if (n == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("ClientSocket::read error");
    }
    return n;
}

ssize_t ClientSocket::write(const char* buf, size_t len) {
    if (state != State::Connected) {
        throw std::runtime_error("Socket not connected, cannot write");
    }
    ssize_t n = ::write(fd, buf, len);
    if (n == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("ClientSocket::write error");
    }
    return n;
}
