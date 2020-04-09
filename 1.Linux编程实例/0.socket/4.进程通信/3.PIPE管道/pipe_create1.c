/*************************************************************************
    > File Name: shm2.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Thu 09 Apr 2020 03:04:38 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>


int main() {
    pid_t pid;
    FILE *fp, *fp1;
    int pipefd[2];
    pipe(pipefd);
	char buff[1024] = {0};
    if ((pid = fork()) < 0) {
        perror("fork");
        return 1;
    }
    fp = fdopen(pipefd[1], "w");
    fp1 = fdopen(pipefd[0], "r");
    if (pid == 0) {
        while (1) {
			//close(pipefd[0]);
            scanf("%[^\n]s", buff);
            getchar();
            fprintf(fp, "%s\n", buff);
			fflush(fp);
        }
    } else {
        while (1) {
			//close(pipefd[1]);
            fscanf(fp1, "%s", buff);
			printf("server : %s\n", buff);
        }
    }
    return 0;
}
