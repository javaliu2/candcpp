#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop* loop, int fd)
    : loop(loop), fd(fd), events(0), revents(0), inEpoll(false) {}

Channel::~Channel() {}

void Channel::handleEvent() {
    if (revents & EPOLLIN) {
        if (readCallback) {
            readCallback();
        }
    }
}

void Channel::setReadCallback(const std::function<void()>& cb) {
    readCallback = cb;
}

void Channel::enableReading() {
    events |= EPOLLIN;
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