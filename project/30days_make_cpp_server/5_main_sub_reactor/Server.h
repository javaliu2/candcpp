#pragma once
#include "EventLoop.h"
#include "ServerSocket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "ThreadPool.h"
#include <map>
/**
 * 主从Reactor结构，reactor的意思是反应堆或者响应器，就是对事件做出反应的东西，即事件驱动响应器
 */
class Server {
private:
    EventLoop* mainReactor;  // 只负责客户端新连接的建立
    Acceptor* acceptor;
    std::map<int, Connection*> connections;
    std::vector<EventLoop*> subReactors;  // 负责已建立连接的客户端 业务请求 处理
    ThreadPool* threadPool;
public:
    Server(EventLoop* loop);
    ~Server();

    void handleNewConnection(ClientSocket* clientSocket);
    void handleRead(int clientFd);
    void handleDeleteConnection(ClientSocket* clientSocket);
    void echoAll();
};