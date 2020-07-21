/************************************************************
    File Name : udp_server.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/05/28 19:08:35
************************************************************/

#include "color.h"
#include "head.h"

int socket_create_udp(int port) {
    int server_listen;

    if ((server_listen = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }

    // printf(BLUE "UDP Socket created." NONE "\n");
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(server_listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    make_nonblock(server_listen);
    if (bind(server_listen, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }

    // printf(GREEN "UDP Socket binded." NONE "\n");
    // printf(PINK "UDP Server listening..." NONE "\n");

    return server_listen;
}