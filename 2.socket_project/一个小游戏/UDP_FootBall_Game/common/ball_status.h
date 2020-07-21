/************************************************************
    File Name : ball_status.h
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/07/02 18:17:43
************************************************************/

#ifndef _BALL_STATUS_H
#define _BALL_STATUS_H

int can_access(struct Point *loc);
int can_kick(struct Point *loc, int strength, char *name);

#endif