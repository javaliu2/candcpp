#include "Server.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <iostream>

Server::Server(EventLoop* loop) : mainReactor(loop), acceptor(nullptr) {
    // 完成服务端监听socket的创建，地址绑定，启动监听；同时完成 监听socket的客户端连接事件 的回调注册
    acceptor = new Acceptor(mainReactor);
    acceptor->setNewConnectionCallback(
        std::bind(&Server::handleNewConnection, this, std::placeholders::_1)
    );
    int size = std::thread::hardware_concurrency();
    threadPool = new ThreadPool(size);
    // 创建了 sub-reactor 对象集合，并且将其loop方法加入线程池，一直运行
    for (int i = 0; i < size; ++i) {
        subReactors.push_back(new EventLoop());
    }
    for (int i = 0; i < size; ++i) {
        // 将 sub-reactor 对应的EventLoop对象中的loop方法加入线程池
        threadPool->addTask(std::bind(&EventLoop::loop, subReactors[i]));
    }
}

Server::~Server() {
    delete acceptor;
    for (auto subReactor : subReactors) {
        subReactor->setQuit();
    }
    for (auto subReactor : subReactors) {
        delete subReactor;
    }
    delete threadPool;
}

/**
 * Acceptor已经接收了连接，这里直接处理clientSocket
 */
void Server::handleNewConnection(ClientSocket* clientSocket) {
    std::cout << "in Server::handleNewConnection(), solve thread id: " << std::this_thread::get_id() << "\n";
    // 使用 sub-reactor 处理新连接请求; 调度策略: 随机
    int idx = clientSocket->getFd() % subReactors.size();
    Connection* conn = new Connection(subReactors[idx], clientSocket);  // 创建连接对象
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
        int client_fd = (*iter).first;
        printf("client_fd %d echo triggered!\n", client_fd);
        printf("content: %s\n", content.c_str());
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
