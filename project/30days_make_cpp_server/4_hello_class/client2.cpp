#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "ClientSocket.h"
#include "InetAddress.h"
#include <iostream>

int main () {
    InetAddress target_addr("127.0.0.1", 9999);
    ClientSocket client(target_addr);
    int sockfd = client.getFd();
    /**
     * client这边也使用epoll模式，监听stdio上的读写事件
     */
    int epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create failed.");
    int fd = STDIN_FILENO;
    int oldflags = fcntl(fd, F_GETFL);  // 获取fd指向的文件状态标志，影响IO
    fcntl(fd, F_GETFD);  // 获取fd本身的状态，影响进程
    fcntl(fd, F_SETFL, oldflags | O_NONBLOCK);
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
    epoll_event ev{};
    ev.events = EPOLLIN | EPOLLET;  // 以ET模式监听stdio上的读事件、写事件
    // 望文生义，理解错了，来自server的数据是在sockfd上的，不会触发stdio的EPOLLOUT，更何况
    // EPOLLOUT表示stdio是否是可写的
    ev.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sockfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    errif(ret == -1, "epoll add failed.");
    
    const int MAX_EVENT = 16;
    epoll_event* events = new epoll_event[MAX_EVENT];

    char buf[1024];
    while (true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENT, -1);
        errif(nfds == -1, "epoll wait error.");
        for (int i = 0; i < nfds; ++i) {
            epoll_event event = events[i];
            int fd = event.data.fd;
            if (fd == STDIN_FILENO && (event.events & EPOLLIN)) {
                int n = read(fd, buf, sizeof(buf));
                if (n > 0) write(sockfd, buf, n);
            } else if (fd == sockfd && (event.events & EPOLLIN)) {
                int n = read(sockfd, buf, sizeof(buf));
                if (n > 0) write(STDOUT_FILENO, buf, n);
            }
        }
    }
    return 0;
}