#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <type_traits> 

int main() {
    // 1、创建一个套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    // 2、bind地址和端口
    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    // 3、启动监听
    listen(sockfd, SOMAXCONN);
    // 4、接收客户端连接
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));
    int client_sockfd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len);
    printf("new client fd: %d! IP: %s; Post: %d\n", client_sockfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    return 0;
}