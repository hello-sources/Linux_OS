/*************************************************************************
	> File Name: send_ctl.c
	> Author: ltw
	> Mail: 3245849061@qq.com
	> Github: https://github.com/hello-sources
	> Created Time: Mon 15 Jun 2020 04:56:17 PM CST
 ************************************************************************/

#include "head.h"

extern int sockfd;;
extern struct FootBallMsg ctl_msg;


void send_ctl() {
    if (ctl_msg.ctl.dirx || ctl_msg.ctl.diry)
        send(sockfd, (void *)&ctl_msg, sizeof(ctl_msg), 0);
    ctl_msg.ctl.dirx = ctl_msg.ctl.diry = 0;
}

