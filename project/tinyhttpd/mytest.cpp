#include <iostream>
#include <netinet/in.h>
#include <iomanip>
#include <cstring>
using namespace std;

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
    
    char buf[40];
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    cout << buf << endl;
    sprintf(buf, "Content-Type: text/html\r\n");  // 覆写，而不是追加
    cout << buf << endl;
    return 0;
}