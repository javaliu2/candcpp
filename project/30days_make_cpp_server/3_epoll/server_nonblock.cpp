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

    setnonblocking(sockfd);  // 设置监听socket为非阻塞
    printf("waiting for client connnect\n");
    int client_socket_fd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len);
    errif(client_socket_fd == -1, "socket accept error");
    printf("client fd: %d\n", client_socket_fd);
    
    char buf[1024];
    setnonblocking(client_socket_fd);  // 设置通信socket为非阻塞
    int n = read(client_socket_fd, buf, sizeof(buf));
    if (n == -1 && errno == EAGAIN) {
        printf("现在没有数据，请稍后再试\n");
    } else if (n > 0) {
        printf("收到数据：%s\n", buf);
    } else if (n == 0) {
        printf("客户端关闭连接");
    }
    close(sockfd);
    return 0;
}