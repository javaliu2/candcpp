#include "Server.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <iostream>

Server::Server(EventLoop* loop) : loop(loop) {
    InetAddress serverAddr("127.0.0.1", 8888);
    ServerSocket* serverSocket = new ServerSocket(serverAddr);  // 完成绑定和监听
    serverSocket->setNonBlocking(true);
    Channel* channel = new Channel(loop, serverSocket->getFd());
    channel->setReadCallback(std::bind(&Server::handleNewConnection, this, serverSocket));
    channel->enableReading();
}

Server::~Server() {}

void Server::handleNewConnection(ServerSocket* serverSocket) {
    InetAddress clientAddr;
    ClientSocket* clientSocket = serverSocket->accept();
    std::cout << "New client connected from " 
              << clientSocket->getAddress().getIp() << ":"
              << clientSocket->getAddress().getPort() << std::endl;
    int clientFd = clientSocket->getFd();
    if (clientFd != -1) {
        Channel* channel = new Channel(loop, clientFd);
        channel->setReadCallback(std::bind(&Server::handleRead, this, clientFd));
        channel->enableReading();
    }
}

void Server::handleRead(int clientFd) {
    char buf[1024];
    while (true) {
        ssize_t n = read(clientFd, buf, sizeof(buf));
        if (n > 0) {
            std::string msg(buf, n);
            std::cout << "Received message from client fd " << clientFd << ": " <<  msg << std::endl;     
            // Echo the message back to the client
            write(clientFd, msg.c_str(), msg.size());
        } else if (n == 0) {
            std::cout << "Client fd " << clientFd << " disconnected." << std::endl;
            close(clientFd);
        } else if (n == -1 && errno == EAGAIN) {
            // No more data to read
            std::cout << "All data read from client fd " << clientFd << std::endl;
            break;
        } else {
            std::cerr << "Read error on client fd " << clientFd << std::endl;
            close(clientFd);
        }
    }
}
