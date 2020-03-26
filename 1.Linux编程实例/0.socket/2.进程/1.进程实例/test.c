/*************************************************************************
    > File Name: test.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 24 Mar 2020 06:56:33 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int pid, x = 0;
	
	for (int i = 1; i <= 10; i++) {
		if ((pid = fork()) < 0) {
			perror("fork()");
			continue;
		}
		if (pid == 0) {
			x = i;
			break;
		}
	}
	printf("I'm %dth child.\n", x);
	sleep(60);
	return 0;
}
