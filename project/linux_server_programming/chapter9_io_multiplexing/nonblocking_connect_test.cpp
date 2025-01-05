/**
 * 代码清单 9-5 非阻塞connect
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

int setnonblocking( int fd ) {
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}
/* 超时连接函数，参数分别是服务器IP地址、端口号和超时时间（毫秒）。
函数成功时返回已经处于连接状态的 socket，失败时返回-1 */
int unblock_connect( const char* ip, int port, int time ) {
    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
    int fdopt = setnonblocking( sockfd );  // user-defined function
    ret = connect( sockfd, ( struct sockaddr* )&address, sizeof( address ) );
    if ( ret == 0 ) {
        /* 如果连接成功，则恢复 sockfd 的属性，并立即返回之 */
        printf( "connect with server immediately.\n" );
        fcntl( sockfd, F_SETFL, fdopt );
        return sockfd;
    } else if ( errno != EINPROGRESS ) {
        /* 如果连接没有立即建立，那么只有当 errno 是 EINPROGRESS 时才表示连接还在进行，否则出错返回 */
        printf ( "unblock connect not support.\n" );
        return -1;
    }

    fd_set writefds;
    struct timeval timeout;

    FD_ZERO( &writefds );
    FD_SET( sockfd, &writefds );

    timeout.tv_sec = time;
    timeout.tv_usec = 0;

    ret = select( sockfd+1, NULL, &writefds, NULL, &timeout );
    if (ret <= 0 ) {
        /* select 超时或者出错，立即返回 */
        printf( "connection time out.\n" );
        close( sockfd );
        return -1;
    }
    if ( ! FD_ISSET( sockfd, &writefds ) ) {
        printf( "no events on sockfd found.\n" );
        close( sockfd );
        return -1;
    }
    int error = 0;
    socklen_t length = sizeof( errno );
    /* 调用 getsockopt 来获取并清除 sockfd 上的错误 */
    if ( getsockopt( sockfd, SOL_SOCKET, SO_ERROR, &error, &length ) < 0 ) {
        printf( "get socket option failed.\n" );
        close( sockfd );
        return -1;
    }
    /* 错误号不为 0 表示连接出错 */
    if ( error != 0 ) {
        printf( "connection failed after select with the error: %d.\n", errno );
        close( sockfd );
        return -1;
    }
    /* 连接成功 */
    printf( "connection ready after select with the socket: %d.\n", sockfd );
    fcntl( sockfd, F_SETFL, fdopt );
    return sockfd;
}

int main( int argc, char* argv[] ) {
    if ( argc <= 2 ) {
        printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );
    
    int sockfd = unblock_connect( ip, port, 0 );
    if ( sockfd < 0 ) {
        return 1;
    }
    close( sockfd );
    return 0;
}