/*************************************************************************
    > File Name: shm1.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 04 Apr 2020 07:08:59 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

int main() {
	pid_t pid;
	int pipefd[2];
	pipe(pipefd);
	char buf[1024] = {0};
	if ((pid = fork()) < 0) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		while (1) {
			close(pipefd[0]);
			scanf("%[^\n]s", buf);
			getchar();
			write(pipefd[1], buf, strlen(buf));
		}
	} else {
		while (1) {
			close(pipefd[1]);
			read(pipefd[0], buf, 1024);
			printf("server : %s\n", buf);
		}
	}
	return 0;
}
