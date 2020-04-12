/*************************************************************************
    > File Name: server.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 07 Apr 2020 06:19:48 PM CST
 ************************************************************************/

#include "../common/color.h"
#include "../common/common.h"
#include "../common/tcp_server.h"
#include "../common/head.h"

#define POLLSIZE 100
#define BUFSIZE 512
char ch_char(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 32;
	}
	return c;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr,"Usage %s port!\n", argv[0]);
		exit(1);
	}
	int server_listen, fd;
	if ((server_listen = socket_create(atoi(argv[1]))) < 0) {
		perror("socket_create");
		exit(1);
	}

	struct pollfd event_set[POLLSIZE];

	for (int i = 0; i < POLLSIZE; i++) {
		event_set[i].fd = -1;
	}
	event_set[0].fd = server_listen;
	event_set[0].events = POLLIN;
	
	while (1) {
		int retval;
		if ((retval = poll(event_set, POLLSIZE, -1)) < 0) {
			perror("poll");
			return 1;
		}
		if (event_set[0].revents & POLLIN) {
			if ((fd = accept(server_listen, NULL, NULL)) < 0) {
				perror("accept");
				continue;
			}
			retval--;
			int i;
			for (i = 1; i < POLLSIZE; i++) {
				if (event_set[i].fd < 0) {
					event_set[i].fd = fd;
					event_set[i].events = POLLIN;
					break;
				}
			}
			if (i == POLLSIZE) {
				printf("Too many clients!\n");
			}
		}
		for (int i = 1; i < POLLSIZE; i++) {
			if (event_set[i].fd < 0) continue;
			if (event_set[i].revents & (POLLIN | POLLHUP | POLLERR)) {
				char buf[BUFSIZE] = {0};
				if (recv(event_set[i].fd, buf, BUFSIZE, 0) < 0) {
					printf("Recv : %s \n", buf);
					for (int i = 0; i < strlen(buf); i++) {
						buf[i] = ch_char(buf[i]);
					}
					send(event_set[i].fd, buf, strlen(buf), 0);
				} else {
					close(event_set[i].fd);
					event_set[i].fd = -1;
				}
			}
			if (retval <= 0) break;
		}
	}

	return 0;
}
