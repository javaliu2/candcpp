#pragma once
#include <string>

class InetAddress;
class Socket {
private:
    int fd;
public:
    Socket();
    Socket(int fd);
    ~Socket();

    void bind(InetAddress*);
    void listen();
    void setnonblocking();

    Socket* accept(InetAddress*);

    void connect(InetAddress*);
    ssize_t read(char* buf, size_t size);
    ssize_t write(const char* buf, size_t size);
    int getFd();
};