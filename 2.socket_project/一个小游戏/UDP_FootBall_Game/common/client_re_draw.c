/************************************************************
    File Name : client_re_draw.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/07/05 16:08:47
************************************************************/

#include <cjson/cJSON.h>

#include "datatype.h"
#include "game.h"
#include "head.h"

extern WINDOW *Football, *Score;
extern struct Map court;

void* __re_draw_court(void* arg);
void re_draw_gate();
void draw_player(cJSON* court_obj, const char* team_color);

// 新建线程重绘球场/球员
void re_draw_court(char* court_json) {
    pthread_t tid;
    char* json = (char*)calloc(2048, 1);
    strcpy(json, court_json);
    pthread_create(&tid, NULL, __re_draw_court, (void*)json);
}

// 解析服务端发来的json 重绘球场/球员
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
            "is_carry": 0,
            "who": 1,
            "name": "sakata",
            "x": 7.243,
            "y": 103.45
        }
    }
*****************************************/
void* __re_draw_court(void* arg) {
    pthread_t tid = pthread_self();
    char* court_json = (char*)arg;

    pthread_detach(tid);
    werase(Football_t);
    box(Football_t, 0, 0);
    box(Football, 0, 0);

    cJSON* court_obj = cJSON_Parse(court_json);
    if (court_obj == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Court JSON Parse Error before: %s\n", error_ptr);
        }
        return NULL;
    }

    // 红队
    wattron(Football_t, COLOR_PAIR(2));
    draw_player(court_obj, "red");
    // 蓝队
    wattron(Football_t, COLOR_PAIR(6));
    draw_player(court_obj, "blue");

    // 球
    wattron(Football_t, COLOR_PAIR(3));
    cJSON* ball_obj = cJSON_GetObjectItemCaseSensitive(court_obj, "ball");
    cJSON* ball_x = cJSON_GetObjectItemCaseSensitive(ball_obj, "x");
    cJSON* ball_y = cJSON_GetObjectItemCaseSensitive(ball_obj, "y");
    if (ball_x == NULL || !cJSON_IsNumber(ball_x)) return NULL;
    if (ball_y == NULL || !cJSON_IsNumber(ball_y)) return NULL;
    w_gotoxy_putc(Football, (int)ball_x->valuedouble, (int)ball_y->valuedouble,
                  'o');
    re_draw_gate();
    wrefresh(Football_t);
    return NULL;
}

// 解析服务端发来的json 重绘记分板
/*****************************************
    格式：
    {
        "score": {
            "red": 2,
            "blue": 3
        },
        "who": 0, // 进球方
        "name": "sakata", // 进球者
    }
*****************************************/
void re_draw_score(const char* const score_json) {
    if (score_json == NULL) return;
    cJSON* score_obj = cJSON_Parse(score_json);
    if (score_obj == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Score JSON Parse Error before: %s\n", error_ptr);
        }
        return;
    }
    cJSON* score_detail = cJSON_GetObjectItemCaseSensitive(score_obj, "score");
    if (score_detail == NULL || !cJSON_IsObject(score_detail)) return;
    cJSON* red_score = cJSON_GetObjectItemCaseSensitive(score_detail, "red");
    cJSON* blue_score = cJSON_GetObjectItemCaseSensitive(score_detail, "blue");
    if (red_score == NULL || !cJSON_IsNumber(red_score)) return;
    if (blue_score == NULL || !cJSON_IsNumber(blue_score)) return;

    // 记分板重绘
    char buff[20] = {0};
    w_gotoxy_puts(Score, 7, 1, "Score");

    sprintf(buff, "%d", blue_score->valueint);
    wattron(Score, COLOR_PAIR(6));
    w_gotoxy_puts(Score, 6, 3, buff);

    wattron(Score, COLOR_PAIR(3));
    w_gotoxy_putc(Score, 9, 3, '-');

    sprintf(buff, "%d", red_score->valueint);
    wattron(Score, COLOR_PAIR(2));
    w_gotoxy_puts(Score, 12, 3, buff);

    wattron(Score, COLOR_PAIR(3));
    wrefresh(Score);
    return;
}

// 绘制球员
void draw_player(cJSON* court_obj, const char* team_color) {
    const cJSON* player = NULL;
    cJSON* team_obj = cJSON_GetObjectItemCaseSensitive(court_obj, team_color);
    if (team_obj == NULL && !cJSON_IsArray(team_obj)) return;
    cJSON_ArrayForEach(player, team_obj) {
        cJSON* name = cJSON_GetObjectItemCaseSensitive(player, "name");
        cJSON* x = cJSON_GetObjectItemCaseSensitive(player, "x");
        cJSON* y = cJSON_GetObjectItemCaseSensitive(player, "y");
        if (name == NULL || !cJSON_IsString(name)) return;
        if (x == NULL || !cJSON_IsNumber(x)) return;
        if (y == NULL || !cJSON_IsNumber(y)) return;
        char p = 'K';
        int pos_x = (int)x->valuedouble, pos_y = y->valuedouble;
        w_gotoxy_putc(Football_t, pos_x, pos_y, p);
        w_gotoxy_puts(Football_t, pos_x + 1, pos_y - 1, name->valuestring);
    }
    return;
}

// 重绘球门
void re_draw_gate() {
    int start = (court.height - court.gate_height) / 2 + 1;
    for (int i = 0; i < court.gate_height; ++i) {
        wattron(Football_t, COLOR_PAIR(6));
        w_gotoxy_putc(Football_t, 1, start + i, 'x');
        wattron(Football_t, COLOR_PAIR(2));
        w_gotoxy_putc(Football_t, court.width + 2, start + i, 'x');
    }
    w_gotoxy_putc(Football_t, 1, 13, 'x');
    return;
}