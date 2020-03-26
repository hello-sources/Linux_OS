/*************************************************************************
    > File Name: auto_run.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Thu 26 Mar 2020 03:32:32 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_result() {
	execl("./exe", "./exe", 0);
}

int main(int argc, char **argv) {
	pid_t pid, w;
	int status;
	if (argc != 2) {
		fprintf(stderr, "Usage : %s [filename].\n", argv[0]);
		exit(0);
	}
	if ((pid = fork()) < 0) {
		perror("fork()");
		exit(0);
	}
	if (pid == 0) {
		execlp("vim", "vim", argv[1], NULL);
	} else {
		if ((w = waitpid(pid, &status, 0)) < 0) {
			perror("waitpid");
			exit(0);
		}
		if ((pid = fork()) < 0) {
			perror("fork()");
			exit(0);
		}
		if (pid == 0) {
			execlp("g++", "g++", argv[1], "-o", "exe", NULL);
			sleep(5);
		} else {
			if ((w = waitpid(pid, &status, 0)) < 0) {
				perror("waitpid");
				exit(1);
			}
		}
			//execl("./exe", "./exe", 0);
	}
	atexit(print_result);
	return 0;
}
