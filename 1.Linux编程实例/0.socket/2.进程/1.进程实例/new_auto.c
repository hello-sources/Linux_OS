/*************************************************************************
    > File Name: new_auto.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Thu 26 Mar 2020 06:14:08 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	pid_t pid;
	char filename[512] = {0};
	char o_name[512] = {0};
	char f_type[256] = {0};//文件类型
	char cmd[25] = {0};
	if (argc < 2) {
		fprintf(stderr, "Usage : %s filename arg...\n", argv[0]);
		return 1;
	}
	strcpy(filename, argv[1]);

	char *sub;
	sub = strrchr(filename, ".");
	strncpy(o_name, filename, sub - filename);
	strcpy(f_type, sub);

	//编译.c文件
	if (!strcmp(f_type, ".c")) {
		strcpy(cmd, "gcc");
	} else if(!strcmp(f_type, ".cpp"))  {
		strcpy(cmd, "g++");
	} else {
		fprintf(stderr, "Not support File Type!\n");
		return 2;
	}

	if ((pid = fork()) < 0) {
		perror("fork()");
		return 3;
	}
	if (pid == 0) {
		execlp("vim", "vim", filename, NULL);
	}
	wait(NULL);

	if ((pid = fork()) < 0) {
		perror("fork()");
		return 3;
	}	
	if (pid == 0) {
		execlp(cmd, cmd, filename, "-o", o_name, NULL);//gcc fileneme -o o_name
	}
	int status;
	wait(&status);
	if (status == 0) {
		printf("OK\n");
		char name[25] = {0};
		sprintf(name, "./%s", o_name);
		execlp(o_name, o_name, NULL);
	} else {
		printf("Failed!\n");
		return 4;
	}
	return 0;
}
