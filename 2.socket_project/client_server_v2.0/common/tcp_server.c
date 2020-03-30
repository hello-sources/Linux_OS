/*************************************************************************
    > File Name: server.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 28 Mar 2020 02:39:49 PM CST
 ************************************************************************/

#include "head.h"
#include "tcp_server.h"

int socket_create(int port) {
	int server_listen;
	if ((server_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}
	printf("Socket create.\n");
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_listen, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("bind");
		return -1;
	}

	printf("Socket bind.\n");
	if (listen(server_listen, 20) < 0) {
		perror("server");
		return -1;
	}
	return server_listen;
}
