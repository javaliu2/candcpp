#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <type_traits> 

template<typename T>
T my_host_to_network_order(T num) {
    static_assert(std::is_integral<T>::value, "Integer type required");
    T result = 0;
    uint8_t* src = reinterpret_cast<uint8_t*>(&num);  // 强制类型转换运算符，用于在不相关类型之间进行底层二进制位的“重新解释”
    uint8_t* dst = reinterpret_cast<uint8_t*>(&result);
    size_t byte_count = sizeof(T);
    for (int i = 0; i < byte_count; ++i) {
        dst[i] = src[byte_count - 1 - i];
    }
    return result;
}

in_addr_t my_inet_addr(const char* ip_str) {
    uint8_t a, b, c, d;
    sscanf(ip_str, "%hhu.%hhu.%hhu.%hhu", &a, &b, &c, &d);
    uint32_t ip_num = (a << 24) | (b << 16) | (c << 8) | d;  // 主机序
    my_host_to_network_order<uint32_t>(ip_num);
}

int main() {
    const char* ip_str = "192.168.1.10";
    in_addr_t net_ip = inet_addr(ip_str);
    printf("inet_addr(\"%s\") = 0x%x\n", ip_str, net_ip);
    printf("my_inet_addr(\"%s\") = 0x%x\n", ip_str, net_ip);
    unsigned char* p = (unsigned char*)&net_ip;
    printf("==== 网络序 ====\n");
    u_int32_t pp = *reinterpret_cast<u_int32_t*>(p);
    printf("IP地址网络形式, 大端序, 高位字节存放在内存低地址处: %02x\n", pp);  // 打印一个数的时候, 内存地址从高到低逐字节打印
    // 所以说，一个数，从左到右读，就是打印出来的顺序
    printf("内存中的顺序： %02x %02x %02x %02x\n", p[0], p[1], p[2], p[3]);
    printf("地址: %p 值: %02x\n", p, p[0]);
    printf("地址: %p 值: %02x\n", p+1, p[1]);
    printf("地址: %p 值: %02x\n", p+2, p[2]);
    printf("地址: %p 值: %02x\n", p+3, p[3]);

    printf("==== 主机序 ====\n");
    in_addr_t host_ip = ntohl(net_ip);
    unsigned char* p2 = (unsigned char*)&host_ip;
    u_int32_t pp2 = *reinterpret_cast<u_int32_t*>(p2);
    printf("IP地址主机形式, 小端序, 高位字节存放在内存高地址处: %02x\n", pp2);
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
    u_int16_t port = 9999;  // 27 0f(hex), 27是高字节序, 0f是低字节序
    uint8_t* port_ptr = reinterpret_cast<uint8_t*>(&port);
    printf("==== 端口号, 主机序 ====\n");
    printf("主机序存储, 0x%02x\n", port);  // 0x270f
    for (size_t i = 0; i < 2; ++i) {
        printf("地址: %p, 值: 0x%02x\n", port_ptr+i, port_ptr[i]);
    }
    uint16_t port_net_order = my_host_to_network_order<uint16_t>(port);
    printf("my_host_to_network_order(%d)=0x%02x\n", port, port_net_order);
    printf("htons(%d)=0x%02x\n", port, htons(port));
    serv_addr.sin_port = htons(port);

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