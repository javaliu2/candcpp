#pragma once
#include <functional>

class EventLoop;
class Channel;
class ServerSocket;
class InetAddress;

class Acceptor {
private:
    EventLoop* loop;
    ServerSocket* serverSocket;
    Channel* channel;
    std::function<void(ServerSocket*)> newConnectionCallback;
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();
    void setNewConnectionCallback(const std::function<void(ServerSocket*)>& cb);
    void acceptConnection();
};