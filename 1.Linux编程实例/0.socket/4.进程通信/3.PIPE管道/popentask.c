/*************************************************************************
    > File Name: popentask.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 14 Apr 2020 06:26:23 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <wait.h>

static pid_t *childpid = NULL;
static int max;

FILE *m_popen(const char *command, const char *type) {
	FILE *fp;
	int pfd[2];
	pid_t pid;
	
	//根据type判断是否退出
	if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
		errno = EINVAL;
		return NULL;
	}
	
	if (childpid == NULL) {
		max = sysconf(_SC_OPEN_MAX);
		if ((childpid = (pid_t *)calloc(max, sizeof(pid_t))) == NULL) {
			return NULL;
		}//说明我们申请childpid数组了
	}
	
	if (pipe(pfd) < 0) {
		return NULL;
	}

	if (pfd[0] >= max || pfd[1] >= max) {
		close(pfd[0]);
		close(pfd[1]);
		return NULL;
	}

	if ((pid = fork()) < 0) {
		return NULL;
	}

	if (pid == 0) {//fork返回值对于子进程来说都是认为0
		if (type[0] == 'r') {
			close(pfd[0]);
			if (pfd[1] != STDOUT_FILENO) {
				dup2(pfd[1], STDOUT_FILENO);
				close(pfd[1]);
			}
		} else {
			close(pfd[1]);
			if (pfd[0] != STDIN_FILENO) {
				dup2(pfd[0], STDIN_FILENO);
				close(pfd[0]);
			}
		}
		for (int i = 0; i <= max; i++) {
			if (childpid[i] > 0) {
				close(i);
			}
		}
		execl("/bin/sh", "sh", "-c", command, NULL);
		exit(0);
	} 
	if (type[0] == 'r') {
		close(pfd[1]);
		if ((fp = fdopen(pfd[0], type)) == NULL) {
			return NULL;
		}
	} else {
		close(pfd[0]);
		if ((fp = fdopen(pfd[1], type)) == NULL) {
			return NULL;
		}
	}

	childpid[fileno(fp)] = pid;
	return fp;
}

int m_pclose(FILE *fp) {
	int status, fd, pid;
	if (childpid == NULL) {
		errno = EINVAL;
		return -1;
	}
	fd = fileno(fp);
	if (fd >= max) {
		errno = EINVAL;
		return -1;
	}
	pid = childpid[fd];
	if (pid == 0) {
		errno = EINVAL;
		return -1;
	}
	childpid[fd] = 0;
	close(fd);
	waitpid(pid, &status, 0);
	return status;
}

int main() {
	FILE *fp;
	char buf[1024] = {0};
	if ((fp = m_popen("ls -al /etc", "r")) == NULL) {
		perror("m_popen");
		return 1;
	}
	while (fgets(buf, 1024, fp) != NULL) {
		printf("%s\n", buf);
	}
	int status = m_pclose(fp);
	printf("status : %d\n", status);
	return 0;
}
