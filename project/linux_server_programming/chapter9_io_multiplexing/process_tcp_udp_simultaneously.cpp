/**
 * 代码清单9-8 同时处理TCP请求和UDP请求的回射服务器
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 1024

int setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

int main ( int argc, char* argv[] ) {
    if ( argc <= 2 ) {
        printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );
    struct sockaddr_in address;
    bzero( &address, sizeof(address) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    /* 创建TCP socket，并将其绑定到端口port上 */
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );
    int ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret != -1 );
    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    /* 创建UDP socket，并将其绑定到端口port上 */
    int udpfd = socket( PF_INET, SOCK_DGRAM, 0 );
    assert( udpfd >= 0 );
    ret = bind( udpfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret != -1 );
    
    int epollfd = epoll_create(5);
    assert(epollfd!=-1);

    /* 注册TCP socket和UDP socket上的可读事件 */
    addfd(epollfd, listenfd);
    addfd(epollfd, udpfd);

    epoll_event events[MAX_EVENT_NUMBER];  // 就绪的事件集合
    while (true) {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (number < 0) {
            printf("epoll failure\n");
            break;
        }
        for (int i = 0; i < number; ++i) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                printf("before accept(), client_addr_len: %d\n", client_addr_len);
                int connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len);
                printf("after accept(), client_addr_len: %d\n", client_addr_len);
                addfd(epollfd, connfd);  // 注册connfd
            } else if (sockfd == udpfd) {
                char buf[UDP_BUFFER_SIZE];
                memset(buf, '\0', UDP_BUFFER_SIZE);
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                ret = recvfrom(udpfd, buf, UDP_BUFFER_SIZE-1, 0, (struct sockaddr*)&client_addr, &client_addr_len);
                if (ret > 0) {
                    /* 回射 */
                    sendto(udpfd, buf, ret, 0, (struct sockaddr*)&client_addr, client_addr_len);
                }
            } else if (events[i].events & EPOLLIN) {
                char buf[TCP_BUFFER_SIZE];
                while (true) {
                    memset(buf, '\0', TCP_BUFFER_SIZE);
                    ret = recv(sockfd, buf, TCP_BUFFER_SIZE-1, 0);
                    if (ret < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        }
                        close(sockfd);
                        break;
                    } else if (ret == 0) {
                        close(sockfd);
                    } else {
                        send(sockfd, buf, ret, 0);
                    }
                }
            } else {
                printf("something else happened\n");
            }
        }
    }
    close(listenfd);
    return 0;    
}