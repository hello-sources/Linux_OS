/*************************************************************************
    > File Name: game.h
    > Author: ltw
    > Mail: 3245849061@qq.com 
    > Created Time: Tue 02 Jun 2020 06:35:35 PM CST
 ************************************************************************/


#ifndef _GAME_H
#define _GAME_H
#include <curses.h>

#define MAX 50

struct LogData {
	char name[20];
	int team; // 0 RED 1 BLUE
};

struct Point {
	int x;
	int y;
};

struct User {
	int team;
	char name[20];
	int flag; // 未响应的次数
	struct sockaddr_in addr;
	struct Point loc;
};

struct Map {
	int width;
	int heigth;
	struct Point start;
	int gate_width;
	int gate_heigth;
};

struct Ctrl {
	int carry;
	int kick;
};

struct TransMsg {
	int dirx;
	int diry;
	struct Ctrl ctrl;	
};

struct Map court;

WINDOW *Football, *Message, *Help, *Score, *Write;

WINDOW *create_newwin(int width, int heigth, int startx, int starty) {
	WINDOW *win;
	win = newwin(heigth, width, starty, startx);
	box(win, 0, 0);
	wrefresh(win);
	move(LINES - 1, 1);
	return win;
}

void destroy_win(WINDOW *win) {
	wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(win);
	delwin(win);
}

void gotoxy(int x, int y) {
	move(y, x);
}

void gotoxy_putc(int x, int y, int c) {
	move(y, x);
	addch(c);
	move(LINES - 1, 1);
	refresh();
}

void gotoxy_puts(int x, int y, char *s) {
	move(y, x);
	addch(s);
	move(LINES - 1, 1);
	refresh();
}

void w_gotoxy_putc(WINDOW *win, int x, int y, int c) {
	mvwaddch(win, y, x, c);
	move(LINES - 1, 1);
	wrefresh(win);
}

void w_gotoxy_puts(WINDOW *win, int x, int y, char *s) {
	mvwprintw(win, y, x, s);
	move(LINES - 1, 1);
	wrefresh(win);
}

void initfootball() {
	initscr();
	clear();
	if (!has_colors() || start_color() == ERR) {
		endwin();
		fprintf(stderr, "终端不支持颜色！\n");
		exit(1);
	}
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	init_pair(5, COLOR_CYAN, COLOR_BLACK);
	init_pair(6, COLOR_BLUE, COLOR_BLACK);
	
	Football = create_newwin(court.width, court.heigth, court.start.x, court.start.y);
	Message = create_newwin(court.width, 5, court.start.x, court.start.y + court.heigth);
	Help = create_newwin(20, court.heigth, court.start.x + court.width, court.start.y);
	Score = create_newwin(20, 5, court.start.x + court.width, court.start.y + court.heigth);
	Write = create_newwin(court.width + 20, 5, court.start.x, court.start.y + court.heigth + 5);
}

void *draw(void *arg) {
	initfootball();
	while (1) {
		sleep(10);
	}
}

#endif
