#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
int main() {
    const char* ip_str = "192.168.1.10";
    in_addr_t net_ip = inet_addr(ip_str);
    printf("inet_addr(\"%s\") = 0x%x\n", ip_str, net_ip);

    unsigned char* p = (unsigned char*)&net_ip;
    printf("==== 网络序 ====\n");
    printf("IP地址网络形式, 大端序, 高位字节存放在内存低地址处: ");
    printf("内存中的顺序： %02x %02x %02x %02x\n", p[0], p[1], p[2], p[3]);
    printf("地址: %p 值: %02x\n", p, p[0]);
    printf("地址: %p 值: %02x\n", p+1, p[1]);
    printf("地址: %p 值: %02x\n", p+2, p[2]);
    printf("地址: %p 值: %02x\n", p+3, p[3]);

    printf("==== 主机序 ====\n");
    in_addr_t host_ip = ntohl(net_ip);
    unsigned char* p2 = (unsigned char*)&host_ip;
    printf("IP地址主机形式, 小端序, 高位字节存放在内存高地址处: ");
    printf("内存中的顺序： %02x %02x %02x %02x\n", p2[0], p2[1], p2[2], p2[3]);
    printf("地址: %p 值: %02x\n", p2, p2[0]);
    printf("地址: %p 值: %02x\n", p2+1, p2[1]);
    printf("地址: %p 值: %02x\n", p2+2, p2[2]);
    printf("地址: %p 值: %02x\n", p2+3, p2[3]);
    /**
     * 192.168.1.10 = c0.a8.01.0a, 左侧是高位，右侧是低位
     * output:
     * inet_addr("192.168.1.10") = 0xa01a8c0
     * ==== 网络序 ====
     * IP地址网络形式, 大端序, 高位字节存放在内存低地址处: 内存中的顺序： c0 a8 01 0a
     * 地址: 0x7fffdede4d64 值: c0
     * 地址: 0x7fffdede4d65 值: a8
     * 地址: 0x7fffdede4d66 值: 01
     * 地址: 0x7fffdede4d67 值: 0a
     * ==== 主机序 ====
     * IP地址主机形式, 小端序, 高位字节存放在内存高地址处: 内存中的顺序： 0a 01 a8 c0
     * 地址: 0x7fffdede4d68 值: 0a
     * 地址: 0x7fffdede4d69 值: 01
     * 地址: 0x7fffdede4d6a 值: a8
     * 地址: 0x7fffdede4d6b 值: c0
     */
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