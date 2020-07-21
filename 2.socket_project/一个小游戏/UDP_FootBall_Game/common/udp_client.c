/************************************************************
    File Name : udp_client.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/04 18:54:38
************************************************************/

#include "head.h"

int socket_udp() {
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    printf("Socket created.\n");
    return sockfd;
}