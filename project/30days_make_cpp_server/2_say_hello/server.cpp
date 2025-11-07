#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include <chrono>
#include <string>

int main () {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    // 设置端口重用
    int opt = 1;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    errif(ret == -1, "set socket port reuse fail");

    struct sockaddr_in sock_addr;
    bzero(&sock_addr, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_port = htons(8888);

    ret = bind(sockfd, (sockaddr*)&sock_addr, sizeof(sock_addr));
    errif(ret == -1, "socket bind error");

    ret = listen(sockfd, SOMAXCONN);
    errif(ret == -1, "socket listen error");

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    bzero(&client_addr, client_addr_len);

    int client_socket_fd = accept(sockfd, (sockaddr*)&client_addr, &client_addr_len);
    errif(client_socket_fd == -1, "socket accept error");

    printf("new client fd %d. IP: %s, Port: %d\n", client_socket_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("go with no error.");
    // 1. 获取当前系统时间（UTC 时间戳，精度到秒）
    auto now = std::chrono::system_clock::now();
    // 2. 转换为 time_t 类型（兼容 C 时间库），再转为本地时间（std::tm 结构体）
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time_t);  // 本地时间（如北京时间）
    // （可选）若需存储为字符串而非直接输出：
    char date_str[20];
    if (std::strftime(date_str, sizeof(date_str), "%Y.%m.%d", &local_tm) == 0) {
        strcpy(date_str, "unknown_date");
    }
    std::string msg2 = "today is " + std::string(date_str) + ", and i want to say good luck.";
    std::string msgs[5] = {"i am server, nice to meet you", msg2, "how are you?", "i have a fulfilling day, and you?", "good bye. see you."};
    int i = 0;
    const int msg_count = 5;
    while (true) {
        char buf[1024];
        bzero(buf, sizeof(buf));
        ssize_t read_bytes = read(client_socket_fd, buf, sizeof(buf));
        if (read_bytes > 0) {
            printf("message from client fd %d: %s\n", client_socket_fd, buf);
            write(client_socket_fd, msgs[i % 5].c_str(), msgs[i % 5].size());
            i++;
        } else if (read_bytes == 0) {
            printf("client fd %d disconnected\n", client_socket_fd);
            close(client_socket_fd);
            break;
        } else if (read_bytes == -1) {
            errif(true, "socket read error");
            close(client_socket_fd);
        }
    }
    close(sockfd);
    return 0;
}