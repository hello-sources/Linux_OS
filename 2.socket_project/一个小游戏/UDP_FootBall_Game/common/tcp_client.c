/************************************************************
    File Name : client.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/03/22 20:21:17
************************************************************/
#include "color.h"
#include "common.h"
#include "head.h"

int socket_connect(char *host, int port) {
    int sockfd;
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    printf(YELLOW "Socket created." NONE "\n");

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        return -1;
    }
    printf(GREEN "Socket connected." NONE "\n");
    return sockfd;
}

int socket_connect_timeout(char *host, int port, long timeout) {
    int sockfd;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        // perror("socket");
        // exit(1);
        return -1;
    }
    printf(YELLOW "Socket created." NONE "\n");

    make_nonblock(sockfd);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout;

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
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error,
                           (socklen_t *)&len) < 0) {
                close(sockfd);
                return -1;
            }
            if (error) {
                close(sockfd);
                return -1;
            }
        } else {
            printf(RED "Connection Timeout!" NONE "\n");
            close(sockfd);
            return -1;
        }
    }
    make_block(sockfd);
    printf(GREEN "Socket connected." NONE "\n");
    return sockfd;
}
