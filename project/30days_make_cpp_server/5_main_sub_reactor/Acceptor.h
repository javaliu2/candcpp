#pragma once
#include <functional>

class EventLoop;
class Channel;
class ServerSocket;
class InetAddress;
class ClientSocket;

class Acceptor {
private:
    EventLoop* loop;
    ServerSocket* serverSocket;
    Channel* channel;
    std::function<void(ClientSocket*)> newConnectionCallback;
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();
    void setNewConnectionCallback(const std::function<void(ClientSocket*)>& cb);
    void acceptConnection();
};