#pragma once
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"
class EventLoop {
private:
    Epoll* epoll;
    bool quit;
    ThreadPool* threadPool;
public:
    EventLoop();
    ~EventLoop();   
    void loop();
    void updateChannel(Channel* channel);
    void addThreadPool(std::function<void()> task);
};