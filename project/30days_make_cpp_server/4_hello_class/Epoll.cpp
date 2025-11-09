#include "Epoll.h"
#include "util.h"
#include <unistd.h>
#include <string.h>

#define MAX_EVENTS 1024

Epoll::Epoll() : epfd(-1), events(nullptr) {
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAX_EVENTS];
    // sizeof(events[0].data);  // 8 bytes
    // sizeof(events[0].events);  // 4 bytes
    bzero(events, sizeof(*events) * MAX_EVENTS);  // 16 bytes，可能有内存对齐
}

Epoll::~Epoll() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

void Epoll::addFd(int fd, uint32_t op, void* ptr) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    if (ptr != nullptr) {
        ev.data.ptr = ptr;
    }
    ev.events = op;
    errif(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev), "epoll add event error");
}

void Epoll::removeFd(int fd) {
    if (fd != -1) {
        ::close(fd);
    }
}
std::vector<epoll_event> Epoll::poll(int timeout) {
    std::vector<epoll_event> activeEvents;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for (int i = 0; i < nfds; ++i) {
        activeEvents.push_back(events[i]);
    }
    return activeEvents;
}