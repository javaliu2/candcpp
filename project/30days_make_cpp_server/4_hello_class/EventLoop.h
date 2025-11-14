#pragma once
#include "Epoll.h"
#include "Channel.h"

class EventLoop {
private:
    Epoll* epoll;
    bool quit;
public:
    EventLoop();
    ~EventLoop();   
    void loop();
    void updateChannel(Channel* channel);
};