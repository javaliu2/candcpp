#include <iostream>
#include <netinet/in.h>
#include <iomanip>
#include <cstring>
using namespace std;

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

/**
 * stat(): status简写, 用于检索指定路径的文件或目录的状态信息, 状态信息存储到struct stat
 */
int stat_test() {
    struct stat st;
    const char *path = "../simpleclient.c";

    if (stat(path, &st) == 0) {  // 成功
        printf("File size: %ld bytes\n", st.st_size);
        printf("File permissions: %o\n", st.st_mode & 0777);
        printf("Last modified time: %ld\n", st.st_mtime);
    } else {
        perror("stat failed");
    }
    return 0;
}


int main() {
    unsigned short int a;
    sizeof(a);  // 2UL
    uint16_t port = 9080;
    cout << "before change, port: " <<  hex << port << endl;  // 0X2378
    uint16_t port_res = htons(port);
    cout << "after change, port: " <<  hex << port_res << endl;  // 0X7823

    uint32_t addr = 0XC0A80001;  // i.e. 192.168.0.1
    uint32_t addr_res = htonl(addr);
    cout << "after change, addr: " << setw(8) << setfill('0') << hex << addr_res << endl;
    
    cout << "\n===sprintf() test===" << endl;
    char buf[40];
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    cout << buf;
    sprintf(buf, "Content-Type: text/html\r\n");  // 覆写，而不是追加
    cout << buf;

    cout << "\n===stat() test===" << endl;
    stat_test();

    cout << "\n===atoi() test===" << endl;
    int num = atoi("24");
    cout << dec << num << endl;
    return 0;
}