#include "Server.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <iostream>

Server::Server(EventLoop* loop) : loop(loop), acceptor(nullptr) {
    // 完成服务端监听socket的创建，地址绑定，启动监听；同时完成 监听socket的客户端连接事件 的回调注册
    acceptor = new Acceptor(loop);
    acceptor->setNewConnectionCallback(
        std::bind(&Server::handleNewConnection1, this, std::placeholders::_1)
    );
}

Server::~Server() {
    delete acceptor;
    // delete loop;  // 不能删除loop，他是传递进来的值（对象），不归Server管
}
/**
 * Server接受连接，完成回调函数注册
 */
void Server::handleNewConnection(ServerSocket* serverSocket) {
    ClientSocket* clientSocket = serverSocket->accept();
    std::cout << "New client connected from " 
              << clientSocket->getAddress().getIp() << ":"
              << clientSocket->getAddress().getPort() << std::endl;
    int clientFd = clientSocket->getFd();
    if (clientFd != -1) {
        Channel* channel = new Channel(loop, clientFd);
        channel->setCallback(std::bind(&Server::handleRead, this, clientFd));
        channel->enableReading();
    }
}
/**
 * Acceptor已经接收了连接，这里直接处理clientSocket
 */
void Server::handleNewConnection1(ClientSocket* clientSocket) {
    Connection* conn = new Connection(loop, clientSocket);  // 创建连接对象
    connections[clientSocket->getFd()] = conn;
    std::function<void(ClientSocket*)> cb = std::bind(&Server::handleDeleteConnection, this, std::placeholders::_1);
    // 设置删除连接对象的回调函数
    conn->setDeleteConnectionCallback(cb);
}

void Server::handleDeleteConnection(ClientSocket *clientSocket) {
    Connection* conn = connections[clientSocket->getFd()];
    connections.erase(clientSocket->getFd());
    delete conn;  // 调用conn的析构，析构函数里又调用了clientSocket的析构，最终clientSocket的fd被关闭，资源被正确释放
}

/**
 * 将服务端标准输入内容回显到所有客户端
 */
void Server::echoAll() {
    // 1、从stdio获取内容
    char buf[1024];
    int n = ::read(STDIN_FILENO, buf, sizeof(buf));
    if (n <= 0) {
        std::cerr << "read from stdio failed\n";
        return;
    }
    std::string content(buf, n);
    // 2、逐一回显
    std::map<int, Connection*>::iterator iter = connections.begin();
    while (iter != connections.end()) {
        Connection* conn = (*iter).second;
        conn->write(content);
        iter++;
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
            break;
        } else if (n == -1 && errno == EAGAIN) {
            // No more data to read
            std::cout << "All data read from client fd " << clientFd << std::endl;
            break;
        } else if (n == -1 && errno == EINTR) {
            std::cout << "Read interrupted for client fd " << clientFd << ", retrying." << std::endl;
            continue; // Interrupted, retry
        }
    }
}
