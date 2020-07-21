/************************************************************
    File Name : client_recver.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/13 14:13:25
************************************************************/

#include "client_re_draw.h"
#include "common.h"
#include "game.h"
#include "head.h"

extern int sockfd;

void *client_recv(void *arg) {
    while (1) {
        curs_set(0);
        struct FootBallMsg msg;
        struct User user;
        bzero(&msg, sizeof(msg));
        recv(sockfd, (void *)&msg, sizeof(msg), 0);
        strcpy(user.name, msg.name);
        user.team = msg.team;

        if (msg.type & FT_TEST) {  // 心跳信息
            DBG(RED "HeartBeat from Server 💗" NONE "\n");
            msg.type = FT_ACK;  // 心跳回执
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        } else if (msg.type & FT_MSG) {  // 用户信息
            DBG(GREEN "Server Msg : " NONE "%s\n", msg.msg);
            Show_Message(, &user, msg.msg, 0);
        } else if (msg.type & FT_WALL) {  // 系统信息
            Show_Message(, NULL, msg.msg, 1);
        } else if (msg.type & FT_FIN) {  // 下线信息
            DBG(GREEN "Server is going to stop!" NONE "\n");
            endwin();
            exit(0);
        } else if (msg.type & FT_GAME) {  // 服务端画面信息
            DBG(L_BLUE "Received a court json: %s" NONE "\n", msg.msg);
            re_draw_court(msg.msg);
        } else if (msg.type & FT_SCORE) {  // 得分信息
            DBG(L_BLUE "Received a score json: %s" NONE "\n", msg.msg);
            re_draw_score(msg.msg);
        } else {
            DBG(GREEN "Server Msg : " NONE "⚠️Unsupported message type.\n");
        }
    }
    return NULL;
}