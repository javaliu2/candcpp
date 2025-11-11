#pragma once
#include "BaseSocket.h"
#include "InetAddress.h"
#include <unistd.h>
#include <stdexcept>

class ClientSocket : public BaseSocket {
public:
    enum class State { Created, Connected };
private:
    InetAddress addr;
    State state;
public:
    ClientSocket();
    explicit ClientSocket(int fd, const InetAddress& addr);
    explicit ClientSocket(const InetAddress& addr);
    ~ClientSocket() override = default;
    // 禁止拷贝
    ClientSocket(const ClientSocket&) = delete;
    ClientSocket& operator=(const ClientSocket&) = delete;
    bool connect();
    ssize_t read(char* buf, size_t len);
    ssize_t write(const char* buf, size_t len);
    
    const InetAddress& getAddress() const { return addr; };
    State getState() const { return state; };
    void setState(State state_) { state = state_; };
    void setAddress(const InetAddress& addr); // 可在构造后设置地址
};
