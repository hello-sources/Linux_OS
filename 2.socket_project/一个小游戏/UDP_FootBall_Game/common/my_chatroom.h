/************************************************************
    File Name : chatroom.h
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/03/29 16:26:53
************************************************************/

#ifndef _CHATROOM_H
#define _CHATROOM_H

#include "head.h"

struct Msg {
    char from[20];
    char to[20];
    int flag;  // 0-公聊 1-私聊 2-通知广播或客户端连接信息 3-断开连接的请求
               // 4-请求在线列表
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