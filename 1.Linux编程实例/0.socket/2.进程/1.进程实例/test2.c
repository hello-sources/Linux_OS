/*************************************************************************
    > File Name: test2.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 24 Mar 2020 07:30:25 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int pid;
	printf("before fork()...");
	if ((pid = fork()) < 0) {
		perror("fork");
	}
	if (pid == 0) {
		printf("\nIn child\n");
	} else {
		sleep(1);
		printf("\nIn parent!\n");
	}
	printf("End fork\n");
	return 0;
}
