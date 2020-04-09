/*************************************************************************
    > File Name: popen_pclose.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Thu 09 Apr 2020 03:49:53 PM CST
 ************************************************************************/


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

pid_t *childpid = NULL;

FILE *my_popen(const char *cmd, const char *type) {
    int pipefd[2];
    FILE *fp;
	pid_t pid;
    pipe(pipefd);
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        if (*type == 'r') {
            close(pipefd[0]);
            if (pipefd[1] != STDOUT_FILENO) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
        } else {
            close(pipefd[1]);
            if (pipefd[0] != STDIN_FILENO) {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
            }
        }
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
    } else {
		wait(NULL);
	}
    if (*type == 'r') {
        close(pipefd[1]);
        if ((fp = fdopen(pipefd[0], type)) == NULL) return NULL;
    } else {
        close(pipefd[0]);
        if ((fp = fdopen(pipefd[1], type)) == NULL) return NULL;
    }
    return fp;
}

int my_pclose(FILE *fp) {
	int fd, stat;
	pid_t pid;
	fd = fileno(fp);
	pid = childpid[fd];
    if (fclose(fp) == EOF) return -1;
	waitpid(pid, &stat, 0);
	return stat;
}

int main() {
    FILE *fp;
    char buf[1024];
    scanf("%[^\n]s", buf);
    getchar();
    fp = my_popen(buf, "w");
	my_pclose(fp);
    return 0;
}

