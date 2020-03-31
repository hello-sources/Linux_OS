/*************************************************************************
    > File Name: common.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 28 Mar 2020 08:20:50 PM CST
 ************************************************************************/

#include "head.h"

char *get_value(char *path, char *key) {
	FILE *fp = NULL;
	ssize_t nrd;
	char *line = NULL, *sub = NULL;
	extern char conf_ans[50];
	size_t linecap;
	if (path == NULL || key == NULL) {
		fprintf(stderr, "Error in arguement!\n");
		return NULL;
	}
	if ((fp = fopen(path, "r")) == NULL) {
		perror("fopen");
		return NULL;
	}
	while ((nrd = getline(&line, &linecap, fp)) != -1) {
		if ((sub = strstr(line, key)) == NULL) {
			continue;
		} else {
			if (line[strlen(key)] == '=') {
				strncpy(conf_ans, sub + strlen(key) + 1, nrd - strlen(key) - 1);
				*(conf_ans + nrd - strlen(key)) = '\0';
				break;
			}
		}
	}
	free(line);
	fclose(fp);
	if (sub == NULL) {
		return NULL;
	}
	return conf_ans;
}

void make_noblock_ioctl(int fd) {
	unsigned long ul = 1;
	ioctl(fd, FIONBIO, &ul);
}

void make_block_ioctl(int fd) {
	unsigned long ul = 0;
	ioctl(fd, FIONBIO, &ul);
}

void make_nonblock(int fd) {
	int flag;
	if ((flag = fcntl(fd, F_GETFL)) < 0) {
		return ;
	}
	flag |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flag);
}

void make_block(int fd) {
	int flag;
	if ((flag = fcntl(fd, F_GETFL)) < 0) {
		return ;
	}
	flag &= ~O_NONBLOCK;
	fcntl(fd, F_SETFL, flag);
}
