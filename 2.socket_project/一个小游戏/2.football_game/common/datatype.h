/*************************************************************************
	> File Name: datatype.h
	> Author: ltw
	> Mail: 3245849061@qq.com
	> Github: https://github.com/hello-sources
	> Created Time: Mon 08 Jun 2020 09:28:22 PM CST
 ************************************************************************/


#ifndef _DATATYPE_H
#define _DATATYPE_H
#define MAXMSG 1024
struct LogRequest{
    char name[20];
    int team; //0 RED 1 BLUE
    char msg[512];
};

struct LogResponse{
    int type; //0 success 1 failed
    char msg[512];
};

struct Point{
    int x;
    int y;
};

struct User{
    int team;
    int fd;
    char name[20];
    int online;
    int flag; // 未响应次数
    struct Point loc;
};

struct Map {
    int width;
    int height;
    struct Point start;
    int gate_width;
    int gate_height;
};

struct Ctrl {
    int carry;
    int kick;
};

struct TransMsg{
    int dirx;
    int diry;
    struct Ctrl ctrl;
};

#define FT_TEST 0x01
#define FT_WALL 0x02
#define FT_MSG 0x04
#define FT_ACK 0x08

struct FootBallMsg {
    int type;
    int size;
    char msg[MAXMSG];
};

#endif
