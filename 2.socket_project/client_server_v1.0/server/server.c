/*************************************************************************
    > File Name: server.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Mon 30 Mar 2020 01:12:54 AM CST
 ************************************************************************/

#include "head.h"
#include "tcp_server.h"


int main(int argc, char **argv) {
	int port, server_listen;
	if (argc != 2) {
		fprintf(stderr, "Usage : %s port!\n", argv[0]);
		return 1;
	}
	port = atoi(argv[1]);
	if ((server_listen = socket_create(port)) < 0) {
		perror("socket_create");
		return 2;
	}

	return 0;
}
