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

    // 使用epoll
    int epoll_fd = epoll_create(10);
    errif(epoll_fd == -1, "epoll create error");

    const int MAX_EVENTS = 1024;
    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));

    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;

    setnonblocking(sockfd);
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev);  // 将sockfd注册到内核事件表中，第四个参数中包括fd是为了当事件触发时，能识别是哪一个fd的事件
    const int READ_BUFFER_SIZE = 1024;
    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll wait error");
        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sockfd) {
                struct sockaddr_in client_addr;
                bzero(&client_addr, sizeof(client_addr));
                socklen_t client_addr_len = sizeof(client_addr);

                int client_socket_fd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len);
                errif(client_socket_fd == -1, "socket accept error");
                printf("new client fd %d. IP:%s, port:%d\n", client_socket_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                bzero(&ev, sizeof(ev));
                ev.data.fd = client_socket_fd;
                ev.events = EPOLLIN | EPOLLET;  // 设置client_socket_fd事件类型为ET，即edge trigger，边沿触发只触发一次
                setnonblocking(client_socket_fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket_fd, &ev);  // 因为内核会拷贝结构体内容，不依赖你的栈内存，所以ev可以复用，但是要保证正确初始化
            } else if (events[i].events & EPOLLIN) {
                char buf[READ_BUFFER_SIZE];
                int client_fd = events[i].data.fd;
                // 由于只触发一次，所以这里要一次性循环读取完全部数据，否则这次读事件就丢失了，数据找不到了
                while (true) {
                    bzero(buf, sizeof(buf));
                    ssize_t bytes_read = read(client_fd, buf, sizeof(buf));  // ET模式下，这里的read必须是非阻塞的，否则程序阻塞在这里，服务端epoll通知不了其他事件的发生（回不到line 54）
                    if (bytes_read > 0) {
                        printf("message from client fd %d: %s\n", client_fd, buf);
                        write(client_fd, buf, sizeof(buf));
                    } else if (bytes_read == -1 && errno == EINTR) {
                        printf("continue reading");
                        continue;
                    } else if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {  // 非阻塞IO，这个条件表示数据全部读取完毕
                        printf("finish reading once, errno:%d\n", errno);
                        break;
                    } else if (bytes_read == 0) {
                        printf("EOF, client fd %d disconnected\n", client_fd);
                        close(client_fd);  // 关闭该socket fd会自动将fd从epoll树上移除
                        break;
                    }
                }
            } else {
                printf("other thing\n");
            }
        }
    }
    close(sockfd);
    return 0;
}