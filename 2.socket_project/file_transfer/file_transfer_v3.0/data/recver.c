/*************************************************************************
	> File Name: recver.c
	> Author: suyelu
	> Mail: suyelu@haizeix.com
	> Created Time: Thu 02 Apr 2020 07:07:22 PM CST
 ************************************************************************/

#include "./common/head.h"
#include "./common/tcp_server.h"
#include "./common/common.h"

struct FileMsg{
        long size;
        char name[50];
        char buf[4096];

};



void child_do(int fd) {
    size_t recv_size;
    struct FileMsg packet_t, packet, packet_pre;
    int packet_size = sizeof(struct FileMsg);
    int offset = 0, flag = 0, cnt = 0;
    long filesize;
    printf("Before recv");
    FILE *fp = NULL;
    while (1) {
        if (flag) {
            memcpy(&packet, &packet_pre, flag);
            offset = flag;
        }
        flag = 0;
        while ((recv_size = recv(fd, (void *)&packet_t, packet_size, 0)) > 0) {
            if (offset + recv_size == packet_size) {
                memcpy((char *)&packet + offset, &packet_t, recv_size);
                offset = 0;
                printf("整包!\n");
                break;
            } else if (offset + recv_size < packet_size) {
                memcpy((char *)&packet + offset, &packet_t, recv_size);
                offset += recv_size;
                printf("拆包!\n");
            } else {
                memcpy((char *)&packet + offset, &packet_t, packet_size - offset);
                flag = recv_size - (packet_size - offset);
                memcpy(&packet_pre, (char *)&packet_t + packet_size - offset, flag);
                printf("粘包!\n");
                offset = 0;
                break;
            }
        }
        if (!cnt++) {
            printf("recv %s with size = %d \n", packet.name, packet_size);
            if ((fp = fopen(packet.name, "wb")) == NULL) {
                perror("fopen");
                break;
            }
        }
    }
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port!\n", argv[0]);
        return 1;
    }
    int server_listen, port, fd;
    pid_t pid;
    port = atoi(argv[1]);
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        return 1;
    }
    while (1) {
        if ((fd = accept(server_listen, NULL, NULL)) < 0) {
            perror("accept");
            continue;
        }
        printf("After accept\n");
        if ((pid = fork()) < 0) {
            perror("fork");
            continue;
        }
        if (pid == 0) {
            close(server_listen);
            child_do(fd);
            //child
        } else {
            //parent
            close(fd);
            wait(NULL);
        }
    }



    return 0;
}

