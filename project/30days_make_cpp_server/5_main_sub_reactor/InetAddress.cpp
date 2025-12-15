#include <arpa/inet.h>
#include "InetAddress.h"
#include "util.h"
#include <string.h>
#include <string>

InetAddress::InetAddress() : addr_len(sizeof(addr)) {
    bzero(&addr, addr_len);
    addr.sin_family = AF_INET;
}

InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(addr)) {
    bzero(&addr, addr_len);
    addr.sin_family = AF_INET;
    int ret = inet_aton(ip, &addr.sin_addr);
    errif(ret == 0, "Invaild IP address in InetAddress constructor");
    addr.sin_port = htons(port);
}

InetAddress::InetAddress(const InetAddress& other) : addr(other.addr), addr_len(other.addr_len) {
    // addr = other.getSockAddr();  // 不必要，因为这里在类的成员函数里，可以直接访问other.addr的成员变量
    // addr = other.addr;  // 结构体默认按值拷贝
    // addr_len = other.addr_len;
}

InetAddress& InetAddress::operator=(const InetAddress& other) {
    if (this != &other) {
        addr = other.addr;
        addr_len = other.addr_len;
    }
    return *this;
}

const sockaddr* InetAddress::getSockAddr() const {
    return (sockaddr*)&addr;
}

socklen_t InetAddress::getSockLen() const {
    return addr_len;
}

sockaddr* InetAddress::getMutableSockAddr() {
    return (sockaddr*)&addr;
}

socklen_t* InetAddress::getMutableSockLen() {
    return &addr_len;
}

std::string InetAddress::getIp() const {
    return std::string(inet_ntoa(addr.sin_addr));
}

uint16_t InetAddress::getPort() const {
    return ntohs(addr.sin_port);
}