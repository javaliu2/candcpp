#include "Epoll.h"
#include "BaseSocket.h"
#include <stdexcept>
#include <iostream>
#include <cstring>
#include "Channel.h"

Epoll::Epoll() {
    epfd = epoll_create1(0);
    if (epfd == -1) throw std::runtime_error("epoll_create1 failed");
    events.resize(MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd != -1) close(epfd);
}

void Epoll::addBaseSocket(BaseSocket* baseSocket, uint32_t eventMask) {
    epoll_event ev{};
    ev.events = eventMask;
    ev.data.ptr = baseSocket;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, baseSocket->getFd(), &ev) == -1) {
        throw std::runtime_error("epoll_ctl add failed");
    }
}

void Epoll::removeBaseSocket(BaseSocket* baseSocket) {
    epoll_ctl(epfd, EPOLL_CTL_DEL, baseSocket->getFd(), nullptr);
}

std::vector<epoll_event> Epoll::poll(int timeout) {
    int n = epoll_wait(epfd, events.data(), MAX_EVENTS, timeout);
    if (n == -1) throw std::runtime_error("epoll_wait failed");
    return std::vector<epoll_event>(events.begin(), events.begin() + n);
}

void Epoll::addFd(int fd, uint32_t eventMask) {
    epoll_event ev{};
    ev.events = eventMask;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error("epoll_ctl add fd failed");
    }
}
void Epoll::updateChannel(Channel* channel) {
    int fd = channel->getFd();
    epoll_event ev{};
    ev.events = channel->getEvents();
    ev.data.ptr = channel;
    if (!channel->isInEpoll()) {
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
            throw std::runtime_error("epoll_ctl add channel failed");
        }
    } else {
        if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1) {
            throw std::runtime_error("epoll_ctl mod channel failed");
        }
    }
}

std::vector<Channel *> Epoll::getActiveChannels(int timeout) {
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(epfd, events.data(), MAX_EVENTS, timeout);
    if (nfds == -1) {
        throw std::runtime_error("epoll_wait failed");
    }
    for (int i = 0; i < nfds; ++i) {
        Channel* channel = static_cast<Channel*>(events[i].data.ptr);
        channel->setRevents(events[i].events);
        activeChannels.push_back(channel);
    }
    return activeChannels;
}
