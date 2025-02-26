/**
 * 代码清单10-1 统一事件源
 */
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_EVENT_NUMBER 1024
static int pipefd[2];

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

/* 信号处理函数 */
void sig_handler(int sig) {
    /* 保留原来的errno，在函数最后恢复，以保证函数的可重入性 */
    int save_errno = errno;
    int msg = sig;
    // todo: msg是int类型，4字节，强转char不会丢失信息嘛，所以说信号值范围是什么
    // 不超过255，所以请放心。respect
    send(pipefd[1], (char*)&msg, 1, 0);  // 将信号值写入管道，以通知主循环
    errno = save_errno;
}
/* 设置信号的处理函数 */
void addsig(int sig) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;  // todo: 查看预编译之后的内容
    /** 预编译之后长这样，sa_handler展开为__sigaction_handler.sa_h  andler
     *     sa.
     *  # 50 "unified_event_source.cpp" 3 4
     *  __sigaction_handler.sa_handler 
     *  # 50 "unified_event_source.cpp"
     *         = sig_handler;
     */
    sa.sa_flags |= SA_RESTART;  // SA_RESTART: 重新调用被该信号终止的系统调用
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd>=0);

    int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret!=-1);

    ret = listen(listenfd, 5);
    assert(ret!=-1);

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd!=-1);
    addfd(epollfd, listenfd);

    /* 使用 socketpair 创建管道，注册 pipefd[0] 上的可读事件 */
    // ret = socketpair(PF_INET, SOCK_STREAM, 0, pipefd);  // ATTENTION! 不是INET
    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
    assert(ret!=-1);
    setnonblocking(pipefd[1]);
    addfd(epollfd, pipefd[0]);
    addsig(SIGHUP);   // 控制终端挂起
    addsig(SIGCHLD);  // 子进程状态发生变化（退出或者暂停）
    addsig(SIGTERM);  // 终止进程
    addsig(SIGINT);   // 键盘输入以中断进程（Ctrl+C）
    bool stop_server = false;
    while (!stop_server) {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (number < 0 && errno != EINTR) {  // EINTR: 中断系统调用
            printf("epoll failure\n");
            break;
        }
        for (int i = 0; i < number; ++i) {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) {
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int connfd = accept(listenfd, (struct sockaddr*)&client_addr,
                                    &client_addr_len);
                addfd(epollfd, connfd);
            } else if (sockfd == pipefd[0] && events[i].events & EPOLLIN) {
                int sig;
                char signals[1024];
                ret = recv(pipefd[0], signals, sizeof(signals), 0);
                printf("number of signal: %d\n", ret);
                if (ret == -1) {
                    continue;
                } else if (ret == 0) {
                    continue;
                } else {
                    /* 因为每个信号值占 1 字节，所以按字节来逐个接收信号。
                    我们以 SIGTERM 为例，来说明如何安全地终止服务器主循环 */
                    for (int i = 0; i < ret; ++i) {
                        switch (signals[i]) {
                            case SIGCHLD:
                            case SIGHUP: {
                                continue;
                            }
                            case SIGTERM:
                            case SIGINT: {
                                stop_server = true;
                            }
                        }
                    }
                }
            } else {
            }
        }
    }
    printf("close fds\n");
    close(listenfd);
    close(pipefd[1]);
    close(pipefd[0]);
    return 0;
}
