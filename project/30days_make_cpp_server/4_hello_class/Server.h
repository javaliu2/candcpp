#pragma once
#include "EventLoop.h"
#include "ServerSocket.h"
#include "Acceptor.h"

class Server {
private:
    EventLoop* loop;
    Acceptor* acceptor;
public:
    Server(EventLoop* loop);
    ~Server();

    void handleNewConnection(ServerSocket* serverSocket);
    void handleRead(int clientFd);
};