/*************************************************************************
    > File Name: cp.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 22 Mar 2020 02:33:49 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 512

int main(int argc, char **argv) {
	int fd_in, fd_out;
	ssize_t nread; 
	char buf[BUFSIZE + 5] = {0};
	if (argc != 3) {
		printf("Usage : %s sourcefile destfile\n", argv[0]);
		exit(1);
	}
	
	if ((fd_in = open(argv[1],O_RDONLY)) == -1) {
		perror(argv[1]);//表示打开文件失败
		exit(1);
	}

	if ((fd_out = creat(argv[2], 0644)) == -1) {
		perror(argv[2]);//表示没有权限创建文件
		exit(1);
	}

	//如果读到了，就开始写
	while ((nread = read(fd_in, buf, sizeof(buf))) > 0) {
		printf("nread = %d\n", nread);
		int nwrite;
		if ((nwrite = write(fd_out, buf, strlen(buf))) != nread) {
			perror("write");
			exit(1);
		}
		memset(buf, 0, BUFSIZE + 5);
		printf("nwrite = %d\n", nwrite);
	}
	
	close(fd_in);
	close(fd_out);
	return 0;
}
