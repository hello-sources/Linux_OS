/*************************************************************************
	> File Name: heart_beat.c
	> Author: ltw
	> Mail: 3245849061@qq.com
	> Github: https://github.com/hello-sources
	> Created Time: Tue 09 Jun 2020 08:21:48 PM CST
 ************************************************************************/

#include "head.h"
#include "datatype.h"
#define MAX 50

extern struct User *rteam, *bteam;

void heart_beat_team(struct User *team) {
    struct FootBallMsg msg;
    msg.type = FT_TEST;
    for (int i = 0; i < MAX; i++) {
        if (team[i].online) {
            if (!team[i].flag) {
                team[i].online = 0;
                continue;
            }
            send(team[i].fd, (void *)&msg, sizeof(msg), 0);
        }
    }
}

void *heart_beat(void *arg) {
    while (1) {
        sleep(10);
        heart_beat_team(bteam);
        heart_beat_team(rteam);
    }
}

