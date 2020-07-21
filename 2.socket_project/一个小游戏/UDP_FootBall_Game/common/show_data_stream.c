/************************************************************
    File Name : show_data_stream.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/30 19:21:18
************************************************************/
#include "head.h"

extern char data_stream[20];
extern WINDOW *Help;
extern struct Map court;

// type: 'l(ogin)', 'c(arry)', 'k(ick)', 's(top)', 'n(normal)', 'e(exit)'
void show_data_stream(int type) {
    for (int i = 18; i > 1; --i) {
        data_stream[i] = data_stream[i - 1];
    }
    data_stream[1] = type;
    for (int i = 1; i < 19; ++i) {
        switch (data_stream[i]) {
            case 'l': {
                wattron(Help, COLOR_PAIR(7));
            } break;
            case 'c': {
                wattron(Help, COLOR_PAIR(8));
            } break;
            case 'k': {
                wattron(Help, COLOR_PAIR(9));
            } break;
            case 's': {
                wattron(Help, COLOR_PAIR(10));
            } break;
            case 'n': {
                wattron(Help, COLOR_PAIR(11));
            } break;
            case 'e': {
                wattron(Help, COLOR_PAIR(12));
            } break;
        }
        mvwprintw(Help, court.height, i, " ");
        wrefresh(Help);
    }
    return;
}