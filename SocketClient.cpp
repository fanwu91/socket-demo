#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_LINE 4096

int main(int argc, char** argv)
{
    int sockfd, n, rec_len;
    char recvline[4094], sendline[4096];
    char buf[MAX_LINE];
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        printf("usage: ./client <ipaddress>\n");
        exit(0);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8000);
    if ((inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) <= 0)
    {
        printf("inet_pton error for %s\n", argv[1]);
        exit(0);
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    printf("send msg to server:\n");
    fgets(sendline, 4096, stdin);
    if (send(sockfd, sendline, strlen(sendline), 0) < 0)
    {
        printf("send msg error: %s(%d)\n", strerror(errno), errno);
        exit(0);
    }

    if ((rec_len = recv(sockfd, buf, MAX_LINE, 0)) == -1)
    {
        perror("recv error\n");
        exit(1);
    }
    buf[rec_len] = '\0';
    printf("Received: %s.", buf);

    close(sockfd);

    return 0;
}
