/************************************************************
    File Name : generate_json.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/07/05 15:10:25
************************************************************/
#include <cjson/cJSON.h>

#include "datatype.h"
#include "game.h"
#include "head.h"
#include "thread_pool.h"

extern struct User *rteam, *bteam;
extern struct Bpoint ball;
extern struct BallStatus ball_status;
extern struct Score score;

void* generate_court_json(void* arg);
void* generate_score_json(void* arg);
cJSON* generate_player_json(struct User* user);

// 发送球场画面json
void send_court_json() {
    pthread_t tid;
    pthread_create(&tid, NULL, generate_court_json, NULL);
    return;
}

// 发送得分信息json
void send_score_json(const char* team_name) {
    pthread_t tid;
    pthread_create(&tid, NULL, generate_score_json, (void*)team_name);
    return;
}

// 生成球场画面json
/*****************************************
    格式：
    {
        "red": [ // 红队队员信息
            {
                "name": "sakata",
                "x": 3,
                "y": 2
            }, ...
        ],
        "blue": [ // 蓝队队员信息
            "name": "sakata",
                "x": 3,
                "y": 2
            }, ...
        ],
        "ball": { // 球信息
            "who": 1,
            "name": "sakata",
            "x": 7.243,
            "y": 103.45
        }
    }
*****************************************/
void* generate_court_json(void* arg) {
    pthread_t tid = pthread_self();
    pthread_detach(tid);
    // 声明最终发送的json对象
    cJSON* court_obj = cJSON_CreateObject();
    if (court_obj == NULL) return NULL;

    // 红队数组
    cJSON* red = cJSON_AddArrayToObject(court_obj, "red");
    if (red == NULL) return NULL;
    for (int i = 0; i < MAX; ++i) {
        if (!rteam[i].online) continue;
        cJSON* player = generate_player_json(&rteam[i]);
        if (player == NULL) return NULL;
        cJSON_AddItemToArray(red, player);
    }

    // 蓝队数组
    cJSON* blue = cJSON_AddArrayToObject(court_obj, "blue");
    if (blue == NULL) return NULL;
    for (int i = 0; i < MAX; ++i) {
        if (!bteam[i].online) continue;
        cJSON* player = generate_player_json(&bteam[i]);
        if (player == NULL) return NULL;
        cJSON_AddItemToArray(blue, player);
    }

    // 球
    cJSON* ball_obj = cJSON_AddObjectToObject(court_obj, "ball");
    if (ball_obj == NULL) return NULL;
    // 哪队带球
    // cJSON* who = cJSON_AddNumberToObject(ball_obj, "who", ball_status.who);
    cJSON* who = cJSON_AddStringToObject(ball_obj, "who", "red");
    if (who == NULL) return NULL;
    // 带球者
    cJSON* name = cJSON_AddStringToObject(ball_obj, "name", ball_status.name);
    if (name == NULL) return NULL;
    // 球坐标
    cJSON* x = cJSON_AddNumberToObject(ball_obj, "x", (int)ball.x);
    cJSON* y = cJSON_AddNumberToObject(ball_obj, "y", (int)ball.y);
    if (x == NULL || y == NULL) return NULL;

    // 广播给客户端
    struct FootBallMsg msg;
    bzero(&msg, sizeof(msg));
    msg.type = FT_GAME;
    strcpy(msg.msg, cJSON_Print(court_obj));
    DBG(L_BLUE "Sended court json:%s" NONE "\n", msg.msg);

    send_all(msg);
    cJSON_Delete(court_obj);
    return NULL;
}

// 生成得分信息json
/*****************************************
    格式：
    {
        "score": {
            "red": 2,
            "blue": 3
        },
        "who": "red", // 进球方
        "name": "sakata", // 进球者
        “score_get": "blue" // 得分方
    }
*****************************************/
void* generate_score_json(void* arg) {
    pthread_t tid = pthread_self();
    pthread_detach(tid);

    cJSON* score_obj = cJSON_CreateObject();
    if (score_obj == NULL) return NULL;

    cJSON* score_detail = cJSON_AddObjectToObject(score_obj, "score");
    if (score_detail == NULL) return NULL;
    cJSON* red_score = cJSON_AddNumberToObject(score_detail, "red", score.red);
    cJSON* blue_score =
        cJSON_AddNumberToObject(score_detail, "blue", score.blue);
    if (red_score == NULL || blue_score == NULL) return NULL;
    cJSON* who = cJSON_AddStringToObject(score_obj, "who",
                                         (ball_status.who ? "blue" : "red"));
    cJSON* name = cJSON_AddStringToObject(score_obj, "name", ball_status.name);
    cJSON* score_get =
        cJSON_AddStringToObject(score_obj, "score_get", (const char*)arg);
    if (who == NULL || name == NULL || score_get == NULL) return NULL;

    // 广播给客户端
    struct FootBallMsg msg;
    bzero(&msg, sizeof(msg));
    msg.type = FT_SCORE;
    strcpy(msg.msg, cJSON_Print(score_obj));
    DBG(L_BLUE "Sended score json:%s" NONE "\n", msg.msg);

    send_all(msg);
    cJSON_Delete(score_obj);
    return NULL;
}

// 工具函数 - 传入一个User 生成一个cjson对象并返回
cJSON* generate_player_json(struct User* user) {
    cJSON* player = cJSON_CreateObject();
    if (player == NULL) return NULL;

    // 玩家名
    cJSON* name = cJSON_AddStringToObject(player, "name", user->name);
    if (name == NULL) return NULL;
    // x坐标
    cJSON* x = cJSON_AddNumberToObject(player, "x", user->loc.x);
    if (x == NULL) return NULL;
    // y坐标
    cJSON* y = cJSON_AddNumberToObject(player, "y", user->loc.y);
    if (y == NULL) return NULL;

    return player;
}
