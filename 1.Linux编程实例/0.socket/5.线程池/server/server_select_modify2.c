/*************************************************************************
    > File Name: server.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 07 Apr 2020 06:19:48 PM CST
 ************************************************************************/


#include "../common/color.h"
#include "../common/common.h"
#include "../common/tcp_server.h"
#include "../common/head.h"

#define CLIENTSIZE 50
#define BUFSIZE 4096


struct  Buffer{
    int fd;
    char buff[BUFSIZE];
    int flag;
    int sendindex;
    int recvindex;
};

struct Buffer *AllocBuffer() {
    struct Buffer *buffer =(struct Buffer *)malloc(sizeof(struct Buffer));
    if (buffer == NULL) 
        return  NULL;
    buffer->fd = -1;
    buffer->flag = buffer->recvindex = buffer->sendindex = 0;
    memset(buffer->buff, 0, BUFSIZE);
    return buffer;
}

void FreeBuffer(struct Buffer *buffer) {
    free(buffer);
}




char ch_char(char c) {
    if (c >= 'a' && c <= 'z')
        return c - 32;
    return  c;
}

int RecvToBuffer(int fd, struct Buffer *buffer) {
    int recv_num;
    while (1) {
        char buff[BUFSIZE] = {0};
        recv_num = recv(fd, buff, sizeof(buff), 0);
        if (recv_num <= 0) break;
        printf("<"L_GREEN"Recv Data"NONE"> : recv data from %d : "L_GREEN"%s"NONE, fd, buff);
        if (buff[recv_num - 1] != 10) printf("\n");
        printf("<"YELLOW"Debug"NONE">Last two char: %d %d\n", buff[recv_num-2], buff[recv_num-1]);
        for (int i = 0; i < recv_num; i++) {
            if (buffer->recvindex < sizeof(buffer->buff))
                buffer->buff[buffer->recvindex++] = ch_char(buff[i]);
            if (buffer->recvindex > 2 && buffer->buff[buffer->recvindex - 1] == 10 && buffer->buff[buffer->recvindex - 3] == 10)
                buffer->flag = 1;
        }
    }
    if (recv_num < 0) {
        if (errno == EAGAIN)
            return 0;
        return -1;
    } else if (recv_num == 0) {
        return 1;
    }
}

int SendFromBuffer(int fd, struct Buffer *buffer) {
    int send_num;
    while (buffer->sendindex < buffer->recvindex){
        printf("<"RED"Send Data"NONE"> : Send data to %d.\n", fd);
        send_num = send(fd, buffer->buff + buffer->sendindex, buffer->recvindex - buffer->sendindex, 0);
        if (send_num < 0) {
            if (errno == EAGAIN)
                return 0;
            buffer->fd = -1;
            return -1;
        }
        buffer->sendindex += send_num;
    }
    if (buffer->sendindex == buffer->recvindex)
        buffer->sendindex = buffer->recvindex = 0;
    buffer->flag = 0;
    return 0;
}

int main(int argc, char ** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port!\n", argv[0]);
        exit(1);
    }
    int server_listen, fd, max_fd;
    if ((server_listen = socket_create(atoi(argv[1]))) < 0) {
        perror("socket_create");
        exit(1);
    }
    struct Buffer *buffer[CLIENTSIZE];    
    
    for (int i = 0; i < CLIENTSIZE; i++) {
        buffer[i] = AllocBuffer();
    }

    make_nonblock(server_listen);

    fd_set rfds, wfds, efds;
    max_fd = server_listen;

    buffer[server_listen]->fd = server_listen;

    while (1) {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_ZERO(&efds);

        FD_SET(server_listen, &rfds);

        printf("<"L_BLUE"Select Start"NONE"> : A new turn for Select.\n");
        printf("<"PINK"Add rfds"NONE">: Set server_listern into rfds!\n");
        for (int i = 0; i < CLIENTSIZE; i++) {
           if (buffer[i]->fd == server_listen) continue;
            if (buffer[i]->fd > 0) {
                if (max_fd < buffer[i]->fd) max_fd =buffer[i]->fd;
                FD_SET(buffer[i]->fd, &rfds);
                printf("<"PINK"Add rfds"NONE">: Set %d into rfds!\n", buffer[i]->fd);
                if (buffer[i]->flag == 1) { 
                    FD_SET(buffer[i]->fd, &wfds);
                    printf("<"L_PINK"Add wfds"NONE">: Set %d into wfds!\n", buffer[i]->fd);
                }
            }
        }

        if (select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0) {
            perror("select");
            return 1;
        }

        printf("<"L_BLUE"Select Return"NONE">Select Return Success.\n");
        if (FD_ISSET(server_listen, &rfds)) {
            printf("<"L_YELLOW"Connect Request"NONE"> : New Client Request for Login.\n");
            if ((fd = accept(server_listen, NULL, NULL)) < 0 ) {
                perror("accept");
                return 1;
            }
            printf("<"GREEN"Connect Success"NONE"> : Login Success with %d\n", fd);
            if (fd > CLIENTSIZE) {
                printf("Too many clients!\n");
                close(fd);
            } else {
                make_nonblock(fd);
                if (buffer[fd]->fd == -1)
                    buffer[fd]->fd = fd;
            }
        }

        for (int i = 0; i <= max_fd; i++) {
            int retval = 0;
            if (i == server_listen || buffer[i]->fd < 0) continue;
            if (FD_ISSET(buffer[i]->fd, &rfds)) {
                retval = RecvToBuffer(i, buffer[i]);
            }
            if (retval == 0 && FD_ISSET(buffer[i]->fd, &wfds)) {
                retval = SendFromBuffer(i, buffer[i]);
            } 
            if (retval) {
                printf("Logout !\n");
                buffer[i]->fd = -1;
                close(i);
            }
        }
    }

    return 0;
} 
