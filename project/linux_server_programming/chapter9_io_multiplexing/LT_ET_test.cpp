/**
 * 代码清单9-3 LT和ET模式
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
#include <stdlib.h>
#include <iostream>
#include <bitset>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10
/* set fd non-block 
* 将文件描述符设置成非阻塞的
*/
int setnonblocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option; 
}
/* register EPOLLIN of fd to kernel event table pointered by epollfd
 * parameter enable_et decide set or notset ET mode of fd
 * 将文件描述符 fd 上的 EPOLLIN 注册到 epollfd 指示的 epoll 内核事件表中，
 * 参数 enable_et 指定是否对 fd 启用 ET 模式
 */
void addfd(int epollfd, int fd, bool enable_et) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;  // 0x001
    if (enable_et) {
        event.events |= EPOLLET;  // 1u << 31
    }
    //std::cout << std::bitset<32>(event.events) << std::endl;  // 10000000 00000000 00000000 00000001
    /**
     * Manipulate an epoll instance "epfd". Returns 0 in case of success,
     * -1 in case of error ( the "errno" variable will contain the
     * specific error code ) The "op" parameter is one of the EPOLL_CTL_*
     * constants defined above. The "fd" parameter is the target of the
     * operation. The "event" parameter describes which events the caller
     * is interested in and any associated user data.
     */
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}
/* LT模式的工作流程 */
void lt( epoll_event* events, int number, int epollfd, int listenfd ) {
    char buf[BUFFER_SIZE];
    printf( "in lt(), number of events is %d\n", number );
    for ( int i = 0; i < number; ++i ) {
        int sockfd = events[i].data.fd;
        if ( sockfd == listenfd ) {
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof( client_addr );
            int connfd = accept( listenfd, ( struct sockaddr* )&client_addr, &client_addr_len );
            printf( "in lt(), connfd is %d\n", connfd );
            addfd( epollfd, connfd, false );  /* 对 connfd 禁用 ET 模式*/
        } else if ( events[i].events & EPOLLIN ) {
            printf( "in lt(), events[i].data.fd is %d\n", events[i].data.fd );
            /* 只要 socket 读缓存中还有未读出的数据，这段代码就被触发 */
            printf( "in lt(), event trigger once\n" );
            memset( buf, '\0', BUFFER_SIZE );
            int ret = recv( sockfd, buf, BUFFER_SIZE-1, 0 );
            if ( ret <= 0 ) {
                close( sockfd );        
                continue;
            }
            printf( "in lt(), get %d bytes of content:_%s_\n", ret, buf );
        } else {
           printf( "in lt(), something else happened.\n" ); 
        }
    }
}
/* ET模式的工作流程 */
void et( epoll_event* events, int number, int epollfd, int listenfd ) {
    char buf[BUFFER_SIZE];
    for ( int i = 0; i < number; ++i ) {
        int sockfd = events[i].data.fd;
        if ( sockfd == listenfd ) {
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof( client_addr );
            int connfd = accept( listenfd, ( struct sockaddr* )&client_addr, &client_addr_len );
            addfd( epollfd, connfd, true );  /* 对 connfd 开启 ET 模式*/
        } else if ( events[i].events & EPOLLIN ) {
            /* 这段代码不会被重复触发，所以我们循环读取数据，以确保把 socket 读缓存中的所有数据读出 */
            printf( "event trigger once.\n" );
            while (1) {
                memset( buf, '\0', BUFFER_SIZE );
                int ret = recv( sockfd, buf, BUFFER_SIZE-1, 0 );
                if ( ret < 0 ) {
                    /* 对于非阻塞 IO，下面的条件成立表示数据已经全部读取完毕，
                    此后，epoll 就能再次触发 sockfd 上的 EPOLLIN 事件，以驱动一下次读操作 */
                    if ( ( errno == EAGAIN ) || ( errno == EWOULDBLOCK ) ) {
                        printf( "read later.\n" );
                        break;
                    }
                    close( sockfd );
                    break;
                } else if ( ret == 0 ) {
                    close( sockfd );
                } else {
                    printf( "get %d bytes of content: _%s_\n", ret, buf );
                }
            }
        } else {
            printf( "something else happened.\n" );
        }
    }
}
typedef union xs_epoll_data
{
  void *ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} xs_epoll_data_t;

int xs_main(int argc, char* argv[]) {
    xs_epoll_data_t data;
    data.fd = 10;
    sizeof(int); // 4UL
    void* ptr;
    sizeof(ptr);  // 8UL
    sizeof(data);  // 8UL, 由最大的数据成员决定
    // printf("*ptr: %ld\n", *(uint64_t*)data.ptr);
    printf("uint32_t: %d\n", data.u32);
    printf("uint64_t: %ld\n", data.u64);

    return 0;
}
int main( int argc, char* argv[] ) { 
    if ( argc <= 2 ) {
		printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi( argv[2] );
	
	int ret = 0;
	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );
	
	int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
	assert( listenfd >= 0 );
    /* 设置端口可重用 */
    int opt = 1;
    setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt) );
	ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
	assert( ret != -1 );
	ret = listen( listenfd, 5 );
	assert( ret != -1 );

    epoll_event events[ MAX_EVENT_NUMBER ];
    /**
     * Creates an epoll instance. Returns an fd for the new instance.
     * The "size" parameter is a hint specifying the number of file
     * descriptors to be associated with the new instance. The fd
     * returned by epoll_create() should be closed with close().
     * 创建一个epoll实例. 返回新实例的fd. 参数 size 是对新实例相关的文件描述符数量的提示.
     * 由 epoll_create() 返回的 fd 应该通过 close() 关闭.
     */
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );
    // 注册服务器监听 socket_fd 上的 EPOLLIN 和 EPOLLET 事件
    addfd( epollfd, listenfd, true );
    printf( "in main(), listenfd is %d\n", listenfd );
    while ( 1 ) {
        /**
         * Wait for events on an epoll instance "epfd". Returns the number of
         * triggered events returned in "events" buffer. Or -1 in case of
         * error with the "errno" variable set to the specific error code. The
         * "events" parameter is a buffer that will contain triggered
         * events. The "maxevents" is the maximum number of events to be
         * returned ( usually size of "events" ). The "timeout" parameter
         * specifies the maximum wait time in milliseconds (-1 == infinite).
         */
        int ret = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        if ( ret < 0 ) {
            printf( "in main(), epoll failure.\n" );
            break;
        }
        //lt( events, ret, epollfd, listenfd );  /* 使用LT模式 */
        et(events, ret, epollfd, listenfd ); /* 使用ET模式 */ 
    }
    close( listenfd );
    return 0;
}
