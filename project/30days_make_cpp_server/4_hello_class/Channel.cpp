#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop* loop, int fd)
    : loop(loop), fd(fd), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {}

void Channel::handleEvent() {
    if (revents & EPOLLIN) {
        if (callback) {
            // 1、主线程执行回调函数
            // callback();
            // 2、使用线程池对回调函数进行执行
            loop->addThreadPool(callback);
        }
    }
}

void Channel::setCallback(const std::function<void()>& cb) {
    callback = cb;
}

void Channel::enableReading() {
    events = EPOLLIN | EPOLLET; // 边沿触发
    loop->updateChannel(this);
}

int Channel::getFd() const {
    return fd;
}

uint32_t Channel::getEvents() const {           
    return events;
}

void Channel::setRevents(uint32_t revt) {
    revents = revt;
}

bool Channel::isInEpoll() const {
    return inEpoll;
}

void Channel::setInEpoll(bool inEpoll) {
    this->inEpoll = inEpoll;
}