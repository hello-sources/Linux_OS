/************************************************************
    File Name : client.c
    Author: ltw 
    Mail: 3245849061@qq.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/04 19:50:22
************************************************************/

#include "../common/head.h"
#include "../common/udp_client.h"

char server_ip[20] = {0};
int server_port = 0;
char *conf = "./football.conf";

int main(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "h:p:")) != -1) {
        switch (opt) {
            case 'h': {
                strcpy(server_ip, optarg);
            } break;
            case 'p': {
                server_port = atoi(optarg);
            } break;
            default: {
                fprintf(stderr, "Usage : %s [-h host] [-p port]\n", argv[0]);
                exit(1);
            }
        }
    }

    argc -= (optind - 1);
    argv += (optind - 1);

    if (argc > 1) {
        fprintf(stderr, "Usage : %s [-h host] [-p port]\n", argv[0]);
        exit(1);
    }

    if (!server_port) server_port = atoi(get_value(conf, "SERVERPORT"));
    if (!strlen(server_ip)) strcpy(server_ip, get_value(conf, "SERVERIP"));

    int sockfd;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip);

    socklen_t len = sizeof(server);

    char buf[512] = {0};
    DBG(GREEN "INFO" NONE " : server_ip = %s, server_port = %d\n", server_ip, server_port);

    if ((sockfd = socket_udp()) < 0) {
        perror("scoket_udp");
        exit(1);
    }

    sendto(sockfd, "Hi", sizeof("Hi"), 0, (struct sockaddr *)&server, len);

    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&server, &len);

    DBG(PINK "RECV" NONE " : %s\n", buf);

    sleep(10);

    return 0;
}
