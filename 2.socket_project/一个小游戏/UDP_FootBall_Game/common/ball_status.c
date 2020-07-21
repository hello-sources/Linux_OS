/************************************************************
    File Name : ball_status.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/07/02 18:18:47
************************************************************/

#include <math.h>

#include "game.h"
#include "head.h"
#define PI 3.1415926

extern WINDOW *Message;
extern struct Bpoint ball;
extern struct BallStatus ball_status;

// 判断玩家是否在球的2*2区域内 如在 返回1 否则0
int can_access(struct Point *loc) {
    // 玩家坐标修正
    int px = loc->x - 2, py = loc->y - 1;
    if (px == (int)ball.x && py == (int)ball.y) {
        return 0;
    }
    if (abs(px - (int)ball.x) > 2 || abs(py - (int)ball.y) > 2) {
        return 0;
    }
    return 1;
}

// 判断玩家是否可以踢球 如可以 更新球速、加速度 返回1 否则0
int can_kick(struct Point *loc, int strength, char *name) {
    // 因为球和人的坐标不在同一个窗口上(人可以出界) 所以先修正坐标
    int px = loc->x - 2, py = loc->y - 1;

    char buff[100] = {0};
    sprintf(buff, "[ORI]x=%d, y=%d  [FIX]x=%d, y=%d  [BALL]x=%d, y = %d",
            loc->x, loc->y, px, py, (int)ball.x, (int)ball.y);
    Show_Message(, , buff, 1);

    if (!can_access(loc)) return 0;  // 不在有效控球范围内

    if (ball_status.is_carry) {                 // 处于带球状态
        if (!strcmp(ball_status.name, name)) {  // 如果是带球者踢的
            ball_status.is_carry = 0;
        } else {  // 如果是他人抢断 则有75%机率抢断成功
            int steal_success = (rand() % 4 < 3);
            if (steal_success) {
                ball_status.is_carry = 0;
            } else {
                return 0;
            }
        }
    }

    double dis = pow(pow((int)ball.x - px, 2) + pow((int)ball.y - py, 2), 0.5);
    double dx = ((int)ball.x - px) / dis;
    double dy = ((int)ball.y - py) / dis;
    double start_v = strength * 40 * 0.2;  // 初始速度力度*40 接触时间0.2

    // 分解为对应坐标轴上的分量
    ball_status.a.x = -3 * dx;  // 空气阻力3
    ball_status.a.y = -3 * dy;
    ball_status.v.x = start_v * dx;
    ball_status.v.y = start_v * dy;

    bzero(buff, sizeof(buff));
    sprintf(buff, "dx=%.2f, dy=%.2f, a.x=%.2f, a.y=%.2f, v.x=%.2f, v.y=%.2f",
            dx, dy, ball_status.a.x, ball_status.a.y, ball_status.v.x,
            ball_status.v.y);
    Show_Message(, , buff, 1);
    return 1;
}