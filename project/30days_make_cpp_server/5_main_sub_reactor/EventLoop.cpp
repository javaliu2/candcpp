#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop()
    : epoll(new Epoll()), quit(false) {}

EventLoop::~EventLoop() {
    delete epoll;
}

void EventLoop::setQuit() {
    quit = true;
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> events = epoll->getActiveChannels();
        for (Channel* channel : events) {
            channel->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* channel) {
    epoll->updateChannel(channel);
}