/*************************************************************************
	> File Name: server_re_draw.c
	> Author: ltw
	> Mail: 3245849061@qq.com
	> Github: https://github.com/hello-sources
	> Created Time: Mon 15 Jun 2020 06:13:51 PM CST
 ************************************************************************/

#include "head.h"
#include "game.h"
#define MAX 50

extern struct User *rteam, *bteam;
extern WINDOW *Football;

void re_draw_player(int team, char *name, struct Point *loc) {
    char p = 'K';
    if (team)
        wattron(Football, COLOR_PAIR(6));
    else 
        wattron(Football, COLOR_PAIR(2));
    w_gotoxy_putc(Football, loc->x, loc->y, p);
    w_gotoxy_puts(Football, loc->x + 1, loc->y - 1, name);
}

void re_draw_team(struct User *team) {
    for (int i = 0; i < MAX; i++) {
        if (!team[i].online) continue;
        re_draw_player(team[i].team, team[i].name, &team[i].loc);
    }
}

void re_draw() {
    wclear(Football);
    box(Football, 0, 0);
    re_draw_team(rteam);
    re_draw_team(bteam);
}
