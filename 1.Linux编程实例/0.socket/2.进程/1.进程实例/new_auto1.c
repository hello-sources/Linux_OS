/*************************************************************************
    > File Name: new_auto1.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Thu 26 Mar 2020 11:54:15 PM CST
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    pid_t pid;
    char filename[512] = {0};
    char name[512] = {0};
    char tmp[5] = {0};
    char cc_flag[50] = {0};

    if (argc != 2) {
        printf("Usage: ./a.out codefile\n");
        exit(1);
    }
    strcpy(filename, argv[1]);
    char *sub;
    sub = strstr(filename, ".");
    strncpy(name, filename, sub - filename);
    strcpy(tmp, sub);

    printf("%s\n", tmp);
    if (!strcmp(tmp, ".c")) {
        strcpy(cc_flag, "gcc");
    } else if (!strcmp(tmp, ".cpp")) {
        strcpy(cc_flag, "g++");
    } else {
        printf("Not Support File Type\n");
        exit(2);
    }
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(3);
    }
    if (pid == 0) {
        execlp("/usr/local/bin/vim", "vim", filename, NULL);
    }
    wait(NULL);
    return 0;
}

