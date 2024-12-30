/** 
 * 代码清单5-12 访问daytime服务
 * 主机 time.nist.gov 提供 daytime service
 * 输出：
 * daytime port is 13
 * ip address: 132.163.97.3
 * the day time is: 
 * 60674 24-12-30 09:07:55 00 0 0 233.4 UTC(NIST) *
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("usage: domain_name\n");
        return 1;
    }
    char* host = argv[1];
    // 获取目标主机地址信息
    // 先在本地/etc/hosts文件找查找，未找到访问DNS服务器
    struct hostent* hostinfo = gethostbyname(host);
    assert(hostinfo);

    // 获取daytime服务信息
    // 通过读取/etc/services文件获取到服务信息
    struct servent* servinfo = getservbyname("daytime", "tcp");
    assert(servinfo);
    printf("daytime port is %d\n", ntohs(servinfo->s_port));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = servinfo->s_port;
    // h_addr_list存储所有ip地址
    // 类型: char **, 二级指针
    // *hostinfo->h_addr_list拿到第一个地址, address1
    // (struct in_addr*)address1, 将地址强转为in_addr结构体类型的指针, ptr
    // *ptr，解引用，拿到真正的ip地址
    // 不需要大小端转换，因为h_addr_list存储的地址是按照网络序的
    address.sin_addr = *(struct in_addr*)*hostinfo->h_addr_list;
    char* ip_addr_str = inet_ntoa(address.sin_addr);
    printf("ip address: %s\n", ip_addr_str);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    char buffer[128];
    ret = read(sockfd, buffer, sizeof(buffer));
    assert(ret > 0);
    buffer[ret] = '\0';
    printf("the day time is: %s\n", buffer);
    close(sockfd);
    return 0;
}