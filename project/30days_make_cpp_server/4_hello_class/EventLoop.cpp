#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop()
    : epoll(new Epoll()), quit(false), threadPool(new ThreadPool()) {}

EventLoop::~EventLoop() {
    delete epoll;
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

void EventLoop::addThreadPool(std::function<void()> task) {
    threadPool->addTask(task);
}
