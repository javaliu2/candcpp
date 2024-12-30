/**
 * Liunx高性能服务器编程（游双）
 * 代码清单5-5 接受一个异常的连接
 * 实验现象：延迟20秒后服务器正常输出客户端连接的IP和端口号
 * 说明：accept只是从监听队列中取出连接，而不论该连接处于何种状态
 */
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // for (int i = 0; i < argc; i++) {
    //     printf("argv[%d]: %s\n", i, argv[i]);
    // }
    if (argc <= 2) {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);  // 字符串转数字
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);  // 主机序转网络序
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(sock, 5);
    assert(ret != -1);
    // 暂停20秒以等待客户端连接和相关操作（掉线或者退出）完成
    sleep(20);
    struct sockaddr_in client;
    socklen_t client_addr_length = sizeof(client);
    int connfd = accept(sock, (struct sockaddr*)&client, &client_addr_length);
    if (connfd < 0) {
        printf("errno is:%d\n", errno);
    } else {
        // 接收连接成功则打印客户端的IP地址和端口号
        char remote[INET_ADDRSTRLEN];
        printf("connected with ip:%s and port:%d\n",
               inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN),
               ntohs(client.sin_port));
        close(connfd);
    }
    close(sock);
    return 0;
}
