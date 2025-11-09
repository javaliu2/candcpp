#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
class InetAddress;

    Socket::Socket() : fd(-1) {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        errif(fd == -1, "Socket::Socket() create error");
    }

    Socket::Socket(int fd) : fd(fd) {
        errif(fd == -1, "Socket::Socket(int) create error");
    }

    Socket::~Socket(){
        if (fd != -1) {
            close(fd);
            fd = -1;
        }
    }

    void Socket::bind(InetAddress* inet_addr) {
        errif(::bind(fd, (sockaddr*)&inet_addr->addr, inet_addr->addr_len) == -1, "Socket::bind(InetAddress) error");
    }

    void Socket::listen() {
        errif(::listen(fd, SOMAXCONN) == -1, "Socket::listen() error");  // ::表示使用全局命名空间的函数
    }
    void Socket::setnonblocking() {
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    }

    Socket* Socket::accept(InetAddress* inet_addr) {
        int client_socket_fd = ::accept(fd, (sockaddr*)&inet_addr->addr, &inet_addr->addr_len);
        errif(client_socket_fd == -1, "Socket::accept(InetAddress) error");
        return new Socket(client_socket_fd);
    }

    ssize_t Socket::read(char* buf, size_t size) {
        int ret = ::read(fd, buf, size);
        errif(ret == -1, "Socket::read(char*, size_t) error");
        return ret;
    }
    ssize_t Socket::write(const char* buf, size_t size) {
        int ret = ::write(fd, buf, size);
        errif(ret == -1, "Socket::write(const char*, size_t) error");
        return ret;
    }
    void Socket::connect(InetAddress* addr) {
        int ret = ::connect(fd, (sockaddr*)&addr->addr, addr->addr_len);
        errif(ret == -1, "Socket::connect(InetAddress*) error");
    }
    int Socket::getFd() {
        return fd;
    }