#pragma once
#include <functional>

class EventLoop;
class Channel;
class ClientSocket;

class Connection {
private:
    EventLoop* loop;
    Channel* channel;
    ClientSocket* socket;
    std::function<void(ClientSocket*)> deleteConnectionCallback;
public:
    Connection(EventLoop* loop, ClientSocket* socket);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(const std::function<void(ClientSocket*)>& cb);
};
