/*************************************************************************
    > File Name: client.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 28 Mar 2020 02:51:34 PM CST
 ************************************************************************/

#include "head.h"
#include "tcp_client.h"
#include "common.h"

int main(int argc, char **argv) {
	int sockfd;
	if (argc != 3) {
		fprintf(stderr, "Usage : %s ip port !\n", argv[0]);
		return 1;
	}

	if ((sockfd = socket_connect(argv[1], atoi(argv[2]))) < 0) {
		perror("socket_connect");
		return 2;
	}
	return 0;
}
