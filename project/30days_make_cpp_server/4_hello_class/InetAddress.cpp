#include <arpa/inet.h>
#include "InetAddress.h"
#include <string.h>

    struct sockaddr_in addr;
    socklen_t addr_len;

    InetAddress::InetAddress() : addr_len(sizeof(addr)) {
        bzero(&addr, addr_len);
    }

    InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(addr)) {
        bzero(&addr, addr_len);
        addr.sin_family = AF_INET;
        inet_aton(ip, &addr.sin_addr);  
        addr.sin_port = htons(port);
    }

    InetAddress::~InetAddress() {

    }
