#include "Acceptor.h"
#include "EventLoop.h"
#include "ServerSocket.h"
#include "Channel.h"
#include <iostream>
#include <thread>

Acceptor::Acceptor(EventLoop* loop)
    : loop(loop),
      serverSocket(new ServerSocket(InetAddress("127.0.0.1", 9999))),
      channel(new Channel(loop, serverSocket->getFd())),
      newConnectionCallback(nullptr) {
    serverSocket->setNonBlocking();
    serverSocket->setReuseAddr();
    channel->setCallback([this]() { this->acceptConnection(); });
    channel->enableReading();
}
Acceptor::~Acceptor() {
    delete channel;
    delete serverSocket;
}
void Acceptor::setNewConnectionCallback(const std::function<void(ClientSocket*)>& cb) {
    newConnectionCallback = cb;
}
/**
 * 接收客户端连接，调用处理连接的回调函数
 */
void Acceptor::acceptConnection() {
    while (true) {
        std::cout << "in Acceptor::acceptConnection(), solve thread id: " << std::this_thread::get_id() << "\n";
        ClientSocket* clientSocket = serverSocket->accept();
        if (!clientSocket) {
            break;
        }
        std::cout << "New client connected from " 
                  << clientSocket->getAddress().getIp() << ":"
                  << clientSocket->getAddress().getPort() << std::endl;
        if (newConnectionCallback) {
            newConnectionCallback(clientSocket);
        }
    }
}