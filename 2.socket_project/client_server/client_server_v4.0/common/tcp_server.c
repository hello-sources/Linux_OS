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

	struct linger m_linger;
	m_linger.l_onoff = 1;
	m_linger.l_linger = 0;
	if (setsockopt(server_listen, SOL_SOCKET, SO_LINGER, &m_linger, (socklen_t)sizeof(m_linger)) < 0) {
		return -1;
	}
	int flag = 1;
	if (setsockopt(server_listen, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) < 0) {
		return -1;
	}

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
