#pragma once
#include "EventLoop.h"
#include "ServerSocket.h"
#include "Acceptor.h"
#include "Connection.h"
#include <map>

class Server {
private:
    EventLoop* loop;
    Acceptor* acceptor;
    std::map<int, Connection*> connections;
public:
    Server(EventLoop* loop);
    ~Server();

    void handleNewConnection(ServerSocket* serverSocket);
    void handleNewConnection1(ClientSocket* clientSocket);
    void handleRead(int clientFd);
    void handleDeleteConnection(ClientSocket* clientSocket);
};