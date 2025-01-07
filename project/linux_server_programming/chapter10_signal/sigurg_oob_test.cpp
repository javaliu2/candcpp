/**
 * 代码清单10-3 用 SIGURG 检测带外数据是否到达
 * client: 使用 oob_send, 数据发送间隔 1 秒
 * server output:
 * SIGURG handler set up successfully.
 * got 3 bytes of normal data: 123
 * SIGURG signal received!
 * got 1 bytes of oob data: c
 * got 2 bytes of normal data: ab
 * got 3 bytes of normal data: 123
 */
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<fcntl.h>

#define BUF_SIZE 1024
static int connfd;
/* SIGURG 信号的处理函数 */
void sig_urg(int sig) {
    printf("SIGURG signal received!\n");
    int save_errno = errno;
    char buf[BUF_SIZE];
    memset(buf, '\0', sizeof(buf));
    int ret = recv(connfd, buf, BUF_SIZE-1, MSG_OOB);
    printf("got %d bytes of oob data: %s\n", ret, buf);
    errno = save_errno;
}
void addsig(int sig, void(*sig_handler)(int)) {
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL)!=-1);
    printf("SIGURG handler set up successfully.\n");
}
int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

    sockaddr_in address;  // 因为是c++, 所以可以选择不写 struct
    bzero(&address, sizeof(address));  // it's strongly necessary
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    int optval = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&optval, sizeof(optval));
    int ret = bind(listenfd, (sockaddr*)&address, sizeof(address));
    ret = listen(listenfd, 5);
    
    sockaddr_in client;
    socklen_t client_len = sizeof(client);

    connfd = accept(listenfd, (sockaddr*)&client, &client_len);
    if (connfd < 0) {
        printf("errno is %d\n", errno);
    } else {
        addsig(SIGURG, sig_urg);
        /* 使用SIGURG信号之前，我们必须设置socket的宿主进程或进程组 */
        if (fcntl(connfd, F_SETOWN, getpid()) == -1) {
            perror("fcntl failed");
        }
        char buf[BUF_SIZE];
        while (true) {
            memset(buf, '\0', BUF_SIZE);
            ret = recv(connfd, buf, BUF_SIZE-1, 0);
            if (ret <= 0) {
                break;
            }
            printf("got %d bytes of normal data: %s\n", ret, buf);
        }
        close(connfd);
    }
    close(listenfd);
    return 0;
}