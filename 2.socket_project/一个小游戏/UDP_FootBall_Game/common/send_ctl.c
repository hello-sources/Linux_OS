/************************************************************
    File Name : send_ctl.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/14 20:08:39
************************************************************/

#include "head.h"

extern int sockfd;
extern struct FootBallMsg ctl_msg;

void send_ctl(int signum) {
    if (!ctl_msg.ctrl.dirx && !ctl_msg.ctrl.diry) return;
    send(sockfd, (void *)&ctl_msg, sizeof(ctl_msg), 0);
    ctl_msg.ctrl.dirx = ctl_msg.ctrl.diry = 0;
    return;
}