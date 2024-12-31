/**
 * 代码清单6-2 Web服务器上的集中写
 * client output:
(base) xs@xslab:~/basic_skill/candcpp$ telnet 127.0.0.1 50000
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
200 OK HTTP/1.1
Content-Length: 60

Unix API is Heady, but fun. Ha, ha, ha.
love & hate coexist.Connection closed by foreign host.
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

#define BUFFER_SIZE 1024
static const char* status_line[2] = {"200 OK", "500 Internal Server Error"};
int main(int argc, char* argv[]) {
    if (argc <= 3) {
        printf("usage: %s ip_address port_number filename\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    const char* filename = argv[3];
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
        // 用于保存HTTP应答的状态行、头部数据以及一个空行的缓冲区
        char header_buf[BUFFER_SIZE];
        memset(header_buf, '\0', BUFFER_SIZE);
        // 用于存放目标文件内容的应用程序缓存
        char* file_buf;
        // 用于获取目标文件的属性，比如是否为目录，文件大小等
        struct stat file_stat;
        // 记录目标文件是否是有效文件
        bool vaild = true;
        // 缓存区header_buf目前已经使用了多少字节的空间
        int len = 0;
        if (stat(filename, &file_stat) < 0) {  // 目标文件不存在
            vaild = false;
        } else {
            if (S_ISDIR(file_stat.st_mode)) {  // 目标文件是一个目录
                vaild = false;
            } else if (file_stat.st_mode & S_IROTH) {  // 当前用户有读取目标文件的权限
                /**
                 * 动态分配缓存区file_buf，并指定其大小为目标文件大小file_stat.st_size加1
                 * 然后将目标文件读入缓存区file_buf中
                 */
                int fd = open(filename, O_RDONLY);
                file_buf = new char[file_stat.st_size + 1];
                memset(file_buf, '\0', file_stat.st_size+1);
                if (read(fd, file_buf, file_stat.st_size) < 0) {
                    vaild = false;
                }
            } else {
                vaild = false;
            }
        }
        // 目标文件有效，发送正常的HTTP应答
        if (vaild) {
            // 设置HTTP应答报文的状态行，"Content-Length"头部字段和一个空行
            ret = snprintf(header_buf, BUFFER_SIZE-1, "%s %s\r\n", status_line[0], "HTTP/1.1");
            len += ret;
            ret = snprintf(header_buf+len, BUFFER_SIZE-1-len, "Content-Length: %ld\r\n", file_stat.st_size);
            len += ret;
            ret = snprintf(header_buf+len, BUFFER_SIZE-1-len, "%s", "\r\n");
            // 利用writev将header_buf和file_buf的内容一并写入
            struct iovec iv[2];
            iv[0].iov_base = header_buf;
            iv[0].iov_len = strlen(header_buf);
            iv[1].iov_base = file_buf;
            iv[1].iov_len = file_stat.st_size;
            /**
             * signature: ssize_t writev(int __fd, const iovec *__iovec, int __count)
             * 将COUNT个由IOVEC指定的内存数据写入FD
             * struct iovec名字表示input/output vector
            */
            ret = writev(connfd, iv, 2);
        } else {  // 目标文件无效，通知客户端服务器发生了内部错误
            ret = snprintf(header_buf, BUFFER_SIZE-1, "%s %s\r\n", status_line[1], "HTTP/1.1");
            len += ret;
            ret = snprintf(header_buf+len, BUFFER_SIZE-1-len, "%s", "\r\n");
            send(connfd, header_buf, strlen(header_buf), 0);
        }
        close(connfd);
        delete[] file_buf;  // 释放内存，避免memory leak
    }
    close(sock);
    return 0;
}