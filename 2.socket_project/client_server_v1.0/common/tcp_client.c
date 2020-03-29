/*************************************************************************
    > File Name: client.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 22 Mar 2020 08:21:57 PM CST
 ************************************************************************/

#include "head.h"

int socket_connect(char *host, int port) {
	int sockfd;
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(host);
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}
	printf("Socket create.\n");
	if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("connect");
		return -1;
	}
	return sockfd;
}
