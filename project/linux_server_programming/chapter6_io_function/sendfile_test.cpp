/**
 * 代码清单6-3 使用sendfile传输文件
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
    if (argc <= 3) {
        printf("usage: %s ip_address port_number filename\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    const char* filename = argv[3];
    // 打开文件、设置信息
    int filefd = open(filename, O_RDONLY);
    assert(filefd > 0);
    struct stat stat_buf;
    /**
     * Get file attributes for the file, device, pipe, or socket
       that file descriptor FD is open on and put them in BUF.
       获取文件描述符FD指定的文件、设备、管道或者套接字的属性，然后将这些属性写入BUF.
     */
    fstat(filefd, &stat_buf);

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
        sendfile(connfd, filefd, NULL, stat_buf.st_size);
        close(connfd);
    }
    close(sock);
    return 0;
}