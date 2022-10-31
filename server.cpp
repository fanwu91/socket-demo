#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char** argv) {
    // create socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd == -1) {
        printf("socket create fail\n");
        return -1;
    }

    // bind socket to ip and port
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(atoi(argv[1]));

    if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) != 0) {
        printf("bind failed\n");
        return -1;
    }

    // listen() makes this process as a server process which can
    // accept other process' requests
    if (listen(listenfd, 5) != 0) {
        printf("listen failed\n");
        close(listenfd);
        return -1;
    }

    return 0;
}
