#include "Connection.h"
#include "Channel.h"
#include "ClientSocket.h"
#include "EventLoop.h"
#include <iostream>
#include <string>

Connection::Connection(EventLoop* loop_, ClientSocket* socket_) : loop(loop_), socket(socket_), channel(nullptr) {
    channel = new Channel(loop, socket->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, socket->getFd());
    channel->setCallback(cb);
    channel->enableReading();
}

Connection::~Connection() {
    delete channel;
    delete socket;
    // 不能删除loop，因为他是全局唯一的，删除之后服务器宕掉了
}

void Connection::echo(int clientFd) {
    std::cout << "in Connection::echo(int clientFd), solve thread id: " << std::this_thread::get_id() << "\n";
    char buf[1024];
    while (true) {
        ssize_t n = ::read(clientFd, buf, sizeof(buf));
        if (n > 0) {
            std::string msg(buf, n);
            std::cout << "Received message from client fd " << clientFd << ": " <<  msg << std::endl;     
            // Echo the message back to the client
            ::write(clientFd, msg.c_str(), msg.size());
        } else if (n == 0) {
            std::cout << "Client fd " << clientFd << " disconnected." << std::endl;
            // close(clientFd);  // 不能这样直接关闭fd，要通过调用Connection的删除连接回调函数
            deleteConnectionCallback(socket);
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

void Connection::setDeleteConnectionCallback(const std::function<void(ClientSocket*)>& cb) {
    deleteConnectionCallback = cb;
}

std::string Connection::read() {
    char buf[1024];
    int ret = socket->read(buf, sizeof(buf));
    if (ret > 0) {
        return std::string(buf, ret);
    } else if (ret == 0) {
        std::cerr << "client" << socket->getFd() << "disconnected.\n";
    } else if (ret == -1 && errno == EAGAIN) {
        std::cerr << "all data has been read.\n";
    } else {
        std::cerr << "other error.\n";
    }
    return nullptr;
}

void Connection::write(std::string content) {
    socket->write(content.c_str(), content.size());
}
