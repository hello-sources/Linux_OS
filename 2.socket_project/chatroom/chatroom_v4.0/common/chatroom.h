/*************************************************************************
	> File Name: chatroom.h
	> Author: suyelu
	> Mail: suyelu@haizeix.com
	> Created Time: 日  3/29 16:26:37 2020
 ************************************************************************/

#ifndef _CHATROOM_H
#define _CHATROOM_H

#include "head.h"

struct Msg {
    char from[20];
    int flag;
    char message[512];
};  



struct RecvMsg {
    struct Msg msg;
    int retval;
};


int chat_send(struct Msg msg, int fd) {
    if (send(fd, (void *)&msg, sizeof(msg), 0) <= 0) {
        return -1;
    }
    return 0;
}

struct RecvMsg chat_recv(int fd) {
    struct RecvMsg tmp;
    memset(&tmp, 0, sizeof(tmp));
    if (recv(fd, &tmp.msg, sizeof(struct Msg), 0) <= 0) {
        tmp.retval = -1;
    }
    return tmp;
}

#define MAX_CLIENT 512
#endif
