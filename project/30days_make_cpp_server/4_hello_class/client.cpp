#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "ClientSocket.h"
#include "InetAddress.h"

int main () {
    InetAddress target_addr("127.0.0.1", 9999);
    ClientSocket client(target_addr);

    while(true){
        char buf[1024];
        bzero(&buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        ssize_t write_bytes = client.write(buf, sizeof(buf));
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = client.read(buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from server: %s\n", buf);
        }else if(read_bytes == 0){
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes == -1){
            errif(true, "socket read error");
        }
    }
    return 0;
}