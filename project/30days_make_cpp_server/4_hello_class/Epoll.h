#pragma once
#include <sys/epoll.h>
#include <vector>
#include "BaseSocket.h"
class Epoll {
private:
    int epfd;
    static const int MAX_EVENTS = 1024;
    std::vector<epoll_event> events;
public:
    Epoll();
    ~Epoll();

    void addBaseSocket(BaseSocket* baseSocket, uint32_t eventMask);
    void removeBaseSocket(BaseSocket* baseSocket);
    std::vector<epoll_event> poll(int timeout = -1);
};