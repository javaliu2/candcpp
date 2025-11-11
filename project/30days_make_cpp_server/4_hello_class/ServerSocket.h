#pragma once
#include <string>
#include "BaseSocket.h"
#include "InetAddress.h"
#include "ClientSocket.h"

class ServerSocket : public BaseSocket {
public:
    ServerSocket();
    explicit ServerSocket(const InetAddress&);
    ~ServerSocket() override = default;

    void bindAddress(const InetAddress&);
    void listen(int backlog = 128);
    ClientSocket accept();
};