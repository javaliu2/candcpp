#pragma once
#include <arpa/inet.h>

class InetAddress {
private:
    struct sockaddr_in addr;
    socklen_t addr_len;
public:
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress() = default;

    const sockaddr* getSockAddr() const;
    socklen_t getSockLen() const;
    sockaddr* getMutableSockAddr();
    socklen_t* getMutableSockLen();
    std::string getIp() const;
    uint16_t getPort() const;
};