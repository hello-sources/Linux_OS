/*************************************************************************
    > File Name: game.h
    > Author: ltw
    > Mail: 3245849061@qq.com 
    > Created Time: Tue 02 Jun 2020 06:35:35 PM CST
 ************************************************************************/

#ifndef _GAME_H
#define _GAME_H

#define  DEFARG(name, default_value) ((#name[0]) ? (name + 0) : default_value)

#define Show_Message(arg0, arg1, arg2, arg3) show_message(DEFARG(arg0, Message), DEFARG(arg1, NULL), arg2, DEFARG(arg3, 0))

#define MAX 50 

struct Map court;

WINDOW *Football, *Message, *Help, *Score, *Write;

WINDOW *create_newwin(int width, int height, int start_x, int start_y);

void destory_win(WINDOW *win);

void gotoxy(int x, int y);

void gotoxy_puc(int x, int y, int c);

void gotoxy_puts(int x, int y, char *s);

void w_gotoxy_putc(WINDOW *win, int x, int y, int c);

void w_gotoxy_puts(WINDOW *win, int x, int y, char *s);

void init_football();

void *draw(void *arg);
void show_message(WINDOW *win, struct User *user, char *msg, int type);
#endif
