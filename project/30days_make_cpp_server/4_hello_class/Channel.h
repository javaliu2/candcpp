#pragma once
#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel {
private:
    EventLoop* loop;  // 全局只有一个EventLoop对象
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> readCallback;
public:
    Channel(EventLoop* loop, int fd);
    ~Channel();
    
    void handleEvent();
    void setReadCallback(const std::function<void()>& cb);
    void enableReading();
    int getFd() const;
    uint32_t getEvents() const;
    void setRevents(uint32_t revt);
    bool isInEpoll() const;
    void setInEpoll(bool inEpoll);
};
