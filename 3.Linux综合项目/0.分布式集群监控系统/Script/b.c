/*************************************************************************
    > File Name: b.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 02 May 2020 02:50:35 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char *buf;
	while (1) {
		buf = (char *)malloc(sizeof(char) * 1024 * 512 * 1024);
		if (buf == NULL) {
			perror("error");
			exit(1);
		}
	}
	memset(buf, 0, sizeof(buf));
	
	while (1) {

	}
	return 0;
}
