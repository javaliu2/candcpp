#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "ServerSocket.h"
#include "InetAddress.h"
#include "util.h"

ServerSocket::ServerSocket() : BaseSocket() {

}

ServerSocket::ServerSocket(const InetAddress& addr) : BaseSocket() {
    bindAddress(addr);
    listen();
}

void ServerSocket::bindAddress(const InetAddress& server_addr) {
    if (::bind(fd, server_addr.getSockAddr(), server_addr.getSockLen()) == -1) {
        perror("ServerSocket::bindAddress() bind error");
        throw std::runtime_error("ServerSocket::bindAddress() bind failed");
    }
}

void ServerSocket::listen(int backlog) {
    if (::listen(fd, backlog) == -1) {
        perror("ServerSocket::listen() listen error");
        throw std::runtime_error("ServerSocket::listen() listen failed");
    }
}

ClientSocket ServerSocket::accept() {
    InetAddress client_addr;
    int client_socket_fd = ::accept(fd, client_addr.getMutableSockAddr(), client_addr.getMutableSockLen());
    if (client_socket_fd == -1) {
        perror("ServerSocket::accept() accept error");
        throw std::runtime_error("ServerSocket::accept() accept failed");
    }
    return ClientSocket(client_addr);
}