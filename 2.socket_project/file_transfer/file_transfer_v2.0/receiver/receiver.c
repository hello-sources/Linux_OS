/*************************************************************************
    > File Name: receiver.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Thu 02 Apr 2020 06:47:30 PM CST
 ************************************************************************/

#include "../common/head.h"
#include "../common/tcp_server.h"
#include "../common/common.h"

struct FileMsg {
	long size;
	char name[50];
	char buf[4096];
};

void child_do(int fd) {
	size_t recv_size;
	struct FileMsg packet_t;
	int packet_size = sizeof(struct FileMsg);
	printf("Before recv!\n");
	while((recv_size = recv(fd, (void *)&packet_t, packet_size, 0)) > 0) {
		printf("recv = %d\n", recv_size);
	}
}


int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage : %s port !\n", argv[0]);
		return 1;
	}
	int server_listen, port, fd;
	pid_t pid;
	port = atoi(argv[1]);
	if ((server_listen = socket_create(port)) < 0) {
		perror("socket_create");
		return 1;
	}
	while (1) {
		if ((fd = accept(server_listen, NULL, NULL)) < 0) {
			perror("accept");
			continue;
		}
		if ((pid = fork()) < 0) {
			perror("fork");
			continue;
		}
		if (pid == 0) {
			close(server_listen);
			child_do(fd);
		} else {
			close(fd);
		}	
	}

	return 0;
}
