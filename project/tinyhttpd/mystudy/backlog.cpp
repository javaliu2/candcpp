#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static bool stop = false;
static void handle_term(int sig) { stop = true; }

int main(int argc, char* argv[]) {
	// 可以通过 kill PID 来发送 SIGTERM 信号
    signal(SIGTERM, handle_term);
    if (argc <= 3) {
        printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);  // Convert a string to an integer.
    int backlog = atoi(argv[3]);
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
	// 将点分十进制表示的ip字符串 转换 为网络字节序，将结果存储于第三个指针变量指向的内存地址
    inet_pton(AF_INET, ip, &address.sin_addr);  
    address.sin_port = htons(port);  // host to network short version, 16bits
    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
	// ATTENTION! 没有调用listen监听端口
	ret = listen(sock, backlog);
	assert(ret != -1);
    while (!stop) {
        sleep(1);
    }
    close(sock);
    return 0;
}