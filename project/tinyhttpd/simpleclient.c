#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char ch = 'A';

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(4000);
    len = sizeof(address);
    result = connect(sockfd, (struct sockaddr *)&address, len);

    if (result == -1)
    {
        perror("oops: client1");
        exit(1);
    }

    char request[] = "GET / HTTP/1.1\r\nHost: local client\r\nConnection: close\r\n\r\n";
    write(sockfd, request, strlen(request));
    
    char response[1024];
    int n = 0;
    while ((n = read(sockfd, response, sizeof(response) - 1)) > 0) {
        response[n] = '\0';
        printf("%s", response);
    }
    if (n < 0) {
        perror("read error");
    }
    // printf("char from server = %c\n", ch);
    close(sockfd);
    exit(0);
}
