#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8000
#define MAX_LINE 4096

using namespace std;

int main(int argc, char** argv)
{
    int socket_fd;
    struct sockaddr_in servaddr;
    // create socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("create socket error: %s(%d)\n", strerror(errno), errno);
        exit(0);
    }

    // bind socket to ip & port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // ip
    servaddr.sin_port = htons(DEFAULT_PORT); // port 8000

    if (bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    // listen() make the process as a server process
    if (listen(socket_fd, 10) == -1)
    {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        exit(0);
    }

    printf("======waiting for client's request======\n");
    int connect_fd;
    char buff[4096];
    int iret;

    while (1)
    {
        // 1st is the socket which already start listening
        // 2nd is the addr which has the client info, if we don't need client info
        // can be ZERO
        // 3rd is the length of addr, ZERO if the 2nd is already ZERO
        if ((connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1)
        {
            printf("accept socket error: %s(errno: %d)\n", strerror(errno), errno);
            continue;
        }

        // server receive data from socket
        iret = recv(connect_fd, buff, MAX_LINE, 0);

        if (iret <= 0) {
            printf("iret = %d\n", iret);
            break;
        }

        printf("receive: %s\n", buff);

        strcpy(buff, "OK");
        if ((iret = send(connect_fd, buff, strlen(buff), 0)) <= 0) {
            printf("send error");
            break;
        }

        printf("send: %s\n", buff);
    }

    close(socket_fd);
    close(connect_fd);

    return 0;
}
