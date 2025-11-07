#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include <chrono>
#include <string>
#include <sys/epoll.h>
#include <fcntl.h>

void setnonblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main () {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    // 设置端口重用
    int opt = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    errif(ret == -1, "set socket port reuse fail");

    struct sockaddr_in sock_addr;
    bzero(&sock_addr, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_port = htons(8888);

    ret = bind(sockfd, (sockaddr*)&sock_addr, sizeof(sock_addr));
    errif(ret == -1, "socket bind error");

    ret = listen(sockfd, SOMAXCONN);
    errif(ret == -1, "socket listen error");

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, client_addr_len);

    int client_socket_fd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len);
    errif(client_socket_fd == -1, "socket accept error");

    // 使用epoll
    int epoll_fd = epoll_create(0);
    errif(epoll_fd == -1, "epoll create error");

    const int MAX_EVENTS = 1024;
    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));

    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;

    setnonblocking(sockfd);
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev);
    close(sockfd);
    return 0;
}