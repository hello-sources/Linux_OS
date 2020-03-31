/*************************************************************************
    > File Name: common.h
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 28 Mar 2020 08:23:19 PM CST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
char *get_value(char *path, char *key);
void make_noblock_ioctl(int fd);
void make_block_ioctl(int fd);
void make_nonblock(int fd);
void make_block(int fd);
#endif
