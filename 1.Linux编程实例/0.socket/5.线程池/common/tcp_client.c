/*************************************************************************
    > File Name: socket_connetc_timeout.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 11 Apr 2020 06:07:50 PM CST
 ************************************************************************/

#include  "head.h"
#include "common.h"

int socket_connect(char *host, int port) {
    int sockfd;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    printf("Socket create.\n");
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        return -1;
    }
    return sockfd;
}

int socket_connect_timeout(char *host, int port, long timeout) {
    int sockfd;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    make_nonblock(sockfd);//设置非阻塞
    struct timeval tv;
    tv.tv.sec = 0;
    tv.tv.usec = timeout;
    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        int retval, error = -1;
        int len = sizeof(int);
        retval = select(sockfd + 1, NULL, &wfds, NULL, &tv);
        if (retval < 0) {
            close(sockfd);
            return -1;
        } else if (retval) {
            //normal
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error,(socklen_t *)&len) < 0) {
                close(sockfd);
                return -1
            }
            if (error) {
                close(sockfd);
                return -1;
            }
        } else {
            //timeout
            printf("Connect TimeOut!\n");
            close(sockfd);
            return -1;
        }
    }
    make_block(sockfd);
    return sockfd;
}

