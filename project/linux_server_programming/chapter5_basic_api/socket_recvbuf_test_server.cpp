#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc <= 3) {
        printf("usage: %s ip_address port_number send_buffer_size\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;  // 本服务器socket地址
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
    int recvbuf_size = atoi(argv[3]);
    int len = sizeof(recvbuf_size);
    // ideas
    // &sizeof(int);  // expression must be an lvalue or a function designator
    // 先设置socket中发送缓冲区大小，然后立即读取
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf_size, sizeof(recvbuf_size));
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf_size, (socklen_t*)&len);
    printf("the tcp receive buffer size after setting is %d\n", recvbuf_size);
    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(sock, 5);
    assert(ret != -1);
    struct sockaddr_in client;
    socklen_t client_address_length = sizeof(client);
    int connfd = accept(sock, (struct sockaddr*)&client, &client_address_length);
    if (connfd < 0) {
        printf("errno is: %d\n", errno);
    } else {
        printf("accepted one connnection\n");
        char buffer[BUFFER_SIZE];
        memset(buffer, '\0', BUFFER_SIZE);
        while (recv(connfd, buffer, BUFFER_SIZE-1, 0) > 0) {
            printf("recv from client: %s\n", buffer);
            // 获取client的socket信息
            // 16777343==hex0100007F,网络序
            printf("client info: %d:%d\n", client.sin_addr.s_addr, client.sin_port);
            memset(buffer, '\0', BUFFER_SIZE-1);
        } 
        close(connfd);
    }
    close(sock);
    return 0;
}