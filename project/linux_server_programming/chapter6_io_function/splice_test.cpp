/**
 * 代码清单6-4 用splice函数实现的回射服务器
 * 注意区别:
 *      1) splice: 拼接、接合
 *      2) split: 分裂、分开
 * client output:
 *  (base) xs@xslab:~/basic_skill/candcpp$ telnet 127.0.0.1 50000
    Trying 127.0.0.1...
    Connected to 127.0.0.1.
    Escape character is '^]'.
    hello splice  # client input
    hello splice  # echo from server
    Connection closed by foreign host.
 */
#include <sys/uio.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <sys/sendfile.h>

int main(int argc, char* argv[]) {

    // assert(0 != 0); // Assertion '0 != 0' failed.
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
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(sock, 5);
    assert(ret != -1);
    struct sockaddr_in client;
    socklen_t client_addr_len = sizeof(client);
    int connfd = accept(sock, (struct sockaddr*)&client, &client_addr_len);
    if (connfd < 0) {
        printf("errno is: %d\n", errno);
    } else {
        int pipefd[2];
        /**
         * Create a one-way communication channel (pipe).
           If successful, two file descriptors are stored in PIPEDES;
           bytes written on PIPEDES[1] can be read from PIPEDES[0].
           Returns 0 if successful, -1 if not.
           创建单方向的通信通道（管道）。
           如果成功，将两个文件描述符存储于PIPEDES；
           PIPEDES[1]用来写数据，PIPEDES[0]用来读数据。
           如果成功返回0，否则返回-1.
         */
        ret = pipe(pipefd);  // 创建管道
        assert(ret != -1);
        // 注意，这里不是 重定向，只是在两个fd之间传输数据
        // 将connfd上流入的数据 定向到 管道写端
        ret = splice(connfd, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
        assert(ret != -1);  // experssion is true, 程序终止
        // 将管道的输出（读端）定向到 confd
        ret = splice(pipefd[0], NULL, connfd, NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
        assert(ret != -1);
        close(connfd);
    }
    close(sock);
    return 0;
}