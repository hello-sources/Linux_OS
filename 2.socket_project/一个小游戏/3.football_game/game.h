/*************************************************************************
    > File Name: game.h
    > Author: ltw
    > Mail: 3245849061@qq.com 
    > Created Time: Tue 02 Jun 2020 06:35:35 PM CST
 ************************************************************************/

#ifndef _GAME_H
#define _GAME_H
#include <curses.h>

#include "common/head.h"

#define MAX 50 

struct Map court;

WINDOW *Football, *Message, *Help, *Score, *Write;

WINDOW *create_newwin(int width, int height, int start_x, int start_y) {
    WINDOW *win;
    win = newwin(height, width, start_y, start_x);
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

void destory_win(WINDOW *win) {
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
    return;
}

void gotoxy(int x, int y) {
    move(y, x);
    return;
}

void gotoxy_puc(int x, int y, int c) {
    move(y, x);
    addch(c);
    move(LINES - 1, 1);
    refresh();
    return;
}

void gotoxy_puts(int x, int y, char *s) {
    move(y, x);
    addstr(s);
    move(LINES - 1, 1);
    refresh();
    return;
}

void w_gotoxy_putc(WINDOW *win, int x, int y, int c) {
    mvwaddch(win, y, x, c);
    move(LINES - 1, 1);
    wrefresh(win);
    return;
}

void w_gotoxy_puts(WINDOW *win, int x, int y, char *s) {
    mvwprintw(win, y, x, s);
    move(LINES - 1, 1);
    wrefresh(win);
    return;
}

void init_football() {
    initscr();
    clear();
    if (!has_colors() || start_color() == ERR) {
        endwin();
        fprintf(stderr, "Your termial not support color!\n");
        exit(1);
    }
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    init_pair(6, COLOR_BLUE, COLOR_BLACK);

    Football =
        create_newwin(court.width, court.height, court.start.x, court.start.y);
    Message = create_newwin(court.width, 5, court.start.x,
                            court.start.y + court.height);
    Help = create_newwin(20, court.height, court.start.x + court.width,
                         court.start.y);
    Score = create_newwin(20, 5, court.start.x + court.width,
                          court.start.y + court.height);
    Write = create_newwin(court.width + 20, 5, court.start.x,
                          court.start.y + court.height + 5);
    return;
}

void *draw(void *arg) {
    init_football();
    while (1) {
        sleep(10);
    }
    return NULL;
}

#endif
