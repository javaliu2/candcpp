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

InetAddress::~InetAddress() {

}

const sockaddr* InetAddress::getSockAddr() const{
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