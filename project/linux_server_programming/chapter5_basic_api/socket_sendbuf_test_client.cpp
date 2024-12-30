#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

int main(int argc, char* argv[]) {
    if (argc <= 3) {
        printf("usage: %s ip_address port_number send_buffer_size\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in server_address;  // 服务器的socket地址
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
    int sendbuf_size = atoi(argv[3]);
    int len = sizeof(sendbuf_size);
    // 先设置socket中发送缓冲区大小，然后立即读取
    setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf_size, sizeof(sendbuf_size));
    getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf_size, (socklen_t*)&len);
    printf("the tcp send buffer size after setting is %d\n", sendbuf_size);
    sizeof(int) != 4;  // 等价于 4 ！= 4, false
    4 != 6;  // legal expression
    // if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address) != -1)) {  // 括号位置有误, 这样相当于传进0
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) != -1) {
        char buffer[BUFFER_SIZE];
        memset(buffer, 'a', BUFFER_SIZE);
        send(sock, buffer, BUFFER_SIZE, 0);
        printf("sended");
    } else {
        printf("connect to server failed\n");
    }
    close(sock);
    return 0;
}