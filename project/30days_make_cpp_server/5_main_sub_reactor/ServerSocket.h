#pragma once
#include <string>
#include "BaseSocket.h"
#include "InetAddress.h"
#include "ClientSocket.h"

class ServerSocket : public BaseSocket {
public:
    enum class State {Created, Bound, Listening};
private:
    InetAddress addr;
    State state;
public:
    ServerSocket();
    explicit ServerSocket(const InetAddress&);
    ~ServerSocket() override = default;

    void bindAddress(const InetAddress&);
    const InetAddress& getAddress() const { return addr; };
    State getState() const { return state; };
    void listen(int backlog = 128);
    // accept 返回动态分配的 ClientSocket*，由调用者管理生命周期
    ClientSocket* accept();
};