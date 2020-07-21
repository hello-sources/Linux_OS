/************************************************************
    File Name : jsontest.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/07/04 15:45:19
************************************************************/

#include <cjson/cJSON.h>
#include <stdio.h>

int main() {
    cJSON *obj = cJSON_CreateObject();
    if (!obj) return 0;
    cJSON *players = cJSON_CreateArray();
    for (int i = 0; i < 5; ++i) {
        char pname[20] = {0};
        sprintf(pname, "player%d", i);

        cJSON *player = cJSON_CreateObject();
        cJSON *name = cJSON_CreateString(pname);
        cJSON *loc = cJSON_CreateObject();
        cJSON *x = cJSON_CreateNumber(i);
        cJSON *y = cJSON_CreateNumber(i);
        cJSON *color = cJSON_CreateNumber(i);

        cJSON_AddItemToObject(loc, "x", x);
        cJSON_AddItemToObject(loc, "y", y);

        cJSON_AddItemToObject(player, "name", name);
        cJSON_AddItemToObject(player, "loc", loc);
        cJSON_AddItemToObject(player, "color", color);

        cJSON_AddItemToArray(players, player);
    }

    cJSON_AddItemToObject(obj, "players", players);
    printf("%s\n", cJSON_Print(obj));
    return 0;
}