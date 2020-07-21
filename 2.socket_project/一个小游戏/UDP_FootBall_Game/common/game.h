/************************************************************
    File Name : game.h
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/02 18:35:46
************************************************************/

#ifndef _GAME_H
#define _GAME_H

// 函数参数默认值
#define DEFARG(name, default_value) ((#name[0]) ? (name + 0) : default_value)

// 带默认参数的show_message 默认向Message打印普通信息
#define Show_Message(arg0, arg1, arg2, arg3)                      \
    show_message(DEFARG(arg0, Message), DEFARG(arg1, NULL), arg2, \
                 DEFARG(arg3, 0))

#include "head.h"
#define MAX 50  // 最大游戏人数/队

struct Map court;

WINDOW *Football, *Football_t, *Message, *Help, *Score, *Write;

char data_stream[20];

WINDOW *create_newwin(int width, int height, int start_x, int start_y);
void destory_win(WINDOW *win);

void gotoxy(int x, int y);
void gotoxy_puc(int x, int y, int c);
void gotoxy_puts(int x, int y, char *s);
void w_gotoxy_putc(WINDOW *win, int x, int y, int c);
void w_gotoxy_puts(WINDOW *win, int x, int y, char *s);
void show_message(WINDOW *win, struct User *user, char *msg, int type);
void show_help_message();

void init_football();
void *draw(void *arg);

#endif