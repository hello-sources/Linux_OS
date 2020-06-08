/*************************************************************************
	> File Name: datatype.h
	> Author: ltw
	> Mail: 3245849061@qq.com
	> Github: https://github.com/hello-sources
	> Created Time: Mon 08 Jun 2020 09:28:22 PM CST
 ************************************************************************/


#ifndef _DATATYPE_H
#define _DATATYPE_H
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
    char name[20];
    int flag; // 未响应次数
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

struct TransMsg{
    int dirx;
    int diry;
    struct Ctrl ctrl;
};

#endif
