#pragma once

class EventLoop;
class ServerSocket;
class Server {
private:
    EventLoop* loop;
public:
    Server(EventLoop* loop);
    ~Server();

    void handleNewConnection(ServerSocket* serverSocket);
    void handleRead(int clientFd);
};