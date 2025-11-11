#include "ServerSocket.h"
#include "ClientSocket.h"
#include "BaseSocket.h"
#include "InetAddress.h"
#include "Epoll.h"
#include <stdio.h>
#include <vector>
#include <string.h>
#include "util.h"
#include <iostream>

int main() {
    InetAddress server_addr("127.0.0.1", 9999);
    ServerSocket server(server_addr);
    server.setNonBlocking();
    std::cout << "i am server.\n";
    std::cout << "i am running on ip: " << server.getAddress().getIp();
    std::cout << ", port: " << server.getAddress().getPort() << std::endl;
    Epoll epoll;
    epoll.addBaseSocket(&server, EPOLLIN | EPOLLET);

    while (true) {   
        auto activeEvents = epoll.poll();
        for (auto& event : activeEvents) {
            if (event.data.ptr == &server) {
                ClientSocket* client = server.accept();
                if (client) {
                    epoll.addBaseSocket(client, EPOLLIN | EPOLLET);
                    std::cout << "new client connected, ip: " << client->getAddress().getIp();
                    std::cout << ", port: " << client->getAddress().getPort() << std::endl;
                }
            } else {
                ClientSocket* client = static_cast<ClientSocket*>(event.data.ptr);
                char buf[1024];
                ssize_t n;
                while ((n = client->read(buf, sizeof(buf))) > 0) {
                    std::cout << "message from client: " << buf << std::endl;  
                    client->write(buf, n);  // echo
                }
                if (n == 0) {
                    epoll.removeBaseSocket(client);
                    delete client;
                }
            }
        }
    }
    return 0;
}