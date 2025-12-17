#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "BaseSocket.h"
#include "InetAddress.h"
#include "util.h"
#include <stdexcept>

ServerSocket::ServerSocket() : BaseSocket(), state(State::Created) {

}

ServerSocket::ServerSocket(const InetAddress& addr) : BaseSocket(), addr(addr), state(State::Created) {
    // this->addr = addr;  // assert 调用InetAdress的赋值函数operator=(), 
    // 不推荐这样做，因为会多调用一次拷贝赋值，推荐的做法是直接在参数初始化列表进行初始化，那样只调用一次拷贝构造
    bindAddress(addr);
    listen();
    // 还有就是这里，上面两个函数要，写在构造函数里面吗，如果写的话，用户又去调用bindAddress那不是会有问题，
    // 要写也可以，设置一个state变量，当socket处于listening状态的时候，不允许bindAddress
    // 所以说，要不要写进来，
}

void ServerSocket::bindAddress(const InetAddress& server_addr) {
    if (state != State::Created) {
        throw std::runtime_error("Cannot bind: socket already bound or listening");
    }
    if (::bind(fd, server_addr.getSockAddr(), server_addr.getSockLen()) == -1) {
        perror("ServerSocket::bindAddress() bind error");
        throw std::runtime_error("ServerSocket::bindAddress() bind failed");
    }
    this->addr = server_addr;  // 拷贝赋值
    state = State::Bound;
}

void ServerSocket::listen(int backlog) {
    if (state != State::Bound) {
        throw std::runtime_error("Cannot listen: socket not bound");
    }
    if (::listen(fd, backlog) == -1) {
        perror("ServerSocket::listen() listen error");
        throw std::runtime_error("ServerSocket::listen() listen failed");
    }
    state = State::Listening;
}

ClientSocket* ServerSocket::accept() {
    if (state != State::Listening) {
        throw std::runtime_error("Cannot accept: socket not listening");
    }
    InetAddress client_addr;
    int client_socket_fd = ::accept(fd, client_addr.getMutableSockAddr(), client_addr.getMutableSockLen());
    if (client_socket_fd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return nullptr;
        }
        perror("ServerSocket::accept() accept error");
        return nullptr;
    }
    auto client = new ClientSocket(client_socket_fd, client_addr);
    client->setNonBlocking();
    client->setState(ClientSocket::State::Connected);
    return client;
}