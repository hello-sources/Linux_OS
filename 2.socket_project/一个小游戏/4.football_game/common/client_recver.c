/*************************************************************************
	> File Name: client_recver.c
	> Author: ltw
	> Mail: 3245849061@qq.com
	> Github: https://github.com/hello-sources
	> Created Time: Sat 13 Jun 2020 02:13:15 PM CST
 ************************************************************************/

#include "head.h"
#include "common.h"
#include "game.h"

extern int sockfd;

void *client_recv(void *arg) {
	while (1) {
		struct FootBallMsg msg;
        struct User user;
		bzero(&msg, sizeof(msg));
		recv(sockfd, (void *)&msg, sizeof(msg), 0);
        strcpy(user.name, msg.name);
        user.team = msg.team;
		if (msg.type & FT_TEST) {
        	DBG(RED"HeartBeat from Server  心跳\n"NONE);
        	msg.type = FT_ACK;
        	send(sockfd, (void *)&msg, sizeof(msg), 0);
		} else if (msg.type & FT_MSG){
        	DBG(GREEN"Server Msg : "NONE"%s\n", msg.msg);
            Show_Message(, &user, msg.msg, 0);
        } else if (msg.type & FT_WALL) {
            Show_Message(, NULL, msg.msg, 1);
		} else if (msg.type & FT_FIN) {
			DBG(GREEN"Server Msg is going to stop.\n "NONE);
			endwin();
			exit(0);
		} else {
			DBG(GREEN"Msg Unsupport\n"NONE);
		}
	}
}
