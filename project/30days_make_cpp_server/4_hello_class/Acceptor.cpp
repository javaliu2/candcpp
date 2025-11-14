#include "Acceptor.h"
#include "EventLoop.h"
#include "ServerSocket.h"
#include "Channel.h"

Acceptor::Acceptor(EventLoop* loop)
    : loop(loop),
      serverSocket(new ServerSocket(InetAddress("127.0.0.1", 9999))),
      channel(new Channel(loop, serverSocket->getFd())),
      newConnectionCallback(nullptr) {
    serverSocket->setNonBlocking(true);
    channel->setReadCallback([this]() { this->acceptConnection(); });
    channel->enableReading();
}
Acceptor::~Acceptor() {
    delete channel;
    delete serverSocket;
}
void Acceptor::setNewConnectionCallback(const std::function<void(ServerSocket*)>& cb) {
    newConnectionCallback = cb;
}
void Acceptor::acceptConnection() {
    if (newConnectionCallback) {
        newConnectionCallback(serverSocket);
    }
}