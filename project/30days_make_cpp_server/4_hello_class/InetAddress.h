#pragma once
#include <arpa/inet.h>
#include <string>

class InetAddress {
private:
    struct sockaddr_in addr;
    socklen_t addr_len;
public:
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress() = default;  // 该函数被声明为default，别的地方不能再提供关于他的定义

    InetAddress(const InetAddress&);
    InetAddress& operator=(const InetAddress&);
    
    const sockaddr* getSockAddr() const;
    socklen_t getSockLen() const;
    sockaddr* getMutableSockAddr();
    socklen_t* getMutableSockLen();
    std::string getIp() const;
    uint16_t getPort() const;
};