/*************************************************************************
    > File Name: datatype.h
    > Author: ltw
    > Mail: 3245849061@qq.com 
    > Created Time: Sat 06 Jun 2020 08:24:06 PM CST
 ************************************************************************/


struct LogRequest {
    char name[20];
    int team;  // 0 RED 1 BLUE
    char msg[512];
};

struct LogResponse {
    int type; //0 success 1 failed
    char msg[512];
};

struct Point {
    int x, y;
};

struct User {
    int team;
    char name[20];
    int flag;
    struct sockaddr_in addr;
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

struct TransMsg {
    int dirx;
    int diry;
    struct Ctrl ctrl;
};

