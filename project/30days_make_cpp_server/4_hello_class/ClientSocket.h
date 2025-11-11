#pragma once
#include "BaseSocket.h"
#include "InetAddress.h"

class ClientSocket : public BaseSocket {
    
public:
    ClientSocket();
    ClientSocket(InetAddress&);
};