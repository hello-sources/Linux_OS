/************************************************************
    File Name : common.h
    Author: ltw
    Mail: 3245849061@qq.com
    Github: https://github.com/Ginakira
    Created Time: 2020/03/28 20:23:43
************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
char conf_ans[50] = {0};
char *get_value(char *path, char *key);
void make_nonblock_ioctl(int fd);
void make_block_ioctl(int fd);
void make_nonblock(int fd);
void make_block(int fd);

#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

#endif
