#include "Socket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include <stdio.h>
#include <vector>
#include <string.h>
#include "util.h"

int main() {
    const char* ip = "127.0.0.1";
    uint16_t port = 9999;
    InetAddress addr(ip, port);
    Socket socket;
    socket.bind(&addr);
    socket.listen();

    // 设置端口重用
    int opt = 1;
    errif(setsockopt(socket.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1, "set socket port reuse fail");

    Epoll epoll;
    socket.setnonblocking();
    epoll.addFd(socket.getFd(), EPOLLIN | EPOLLET, nullptr);

    while (true) {
        std::vector<epoll_event> active_events = epoll.poll();
        for (int i = 0; i < active_events.size(); ++i) {
            epoll_event event = active_events[i];
            if (event.data.fd == socket.getFd()) {
                InetAddress client_addr; // 这里要用new吗, G-bro说不用，只是为了填充地址和端口数据使用
                Socket* client_socket_ptr = socket.accept(&client_addr);
                printf("new client fd %d\n", client_socket_ptr->getFd());
                client_socket_ptr->setnonblocking();
                epoll.addFd(client_socket_ptr->getFd(), EPOLLIN | EPOLLET, (void*)client_socket_ptr);
            } else if (event.events & EPOLLIN) {
                Socket* client_socket_ptr = (Socket*)event.data.ptr;
                char buf[128];
                int client_socket_fd = client_socket_ptr->getFd();
                int ret = -1;
                while (true) {
                    ::bzero(buf, sizeof(buf));
                    ret = client_socket_ptr->read(buf, sizeof(buf));
                    if (ret > 0) {
                        printf("client fd %d, message: %.*s\n", client_socket_fd, ret, buf);
                        client_socket_ptr->write(buf, sizeof(buf));
                    } else if (ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
                        printf("数据完全被读取\n");
                        break;
                    } else if (ret == 0) {
                        printf("EOF, client fd %d disconnected\n", client_socket_fd);
                        epoll.removeFd(client_socket_fd);
                        delete client_socket_ptr;
                        break;
                    }
                }
            } else {
                printf("other thing\n");
            }
        }
    }   
    return 0;
}