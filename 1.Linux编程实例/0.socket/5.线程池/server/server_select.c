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

#define CLIENTSIZE 100
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
	int server_listen, fd, max_fd;
	int client[CLIENTSIZE] = {0};
	memset(client, -1, sizeof(client));
	if ((server_listen = socket_create(atoi(argv[1]))) < 0) {
		perror("socket_create");
		exit(1);
	}
	
	make_nonblock(server_listen);

	fd_set rfds, wfds, efds;
	max_fd = server_listen;

	while (1) {
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);

		FD_SET(server_listen, &rfds);

		for (int i = 0; i < CLIENTSIZE; i++) {
			if (client[i] == server_listen) continue;
			if (client[i] > 0) {
				if (max_fd < client[i]) max_fd = client[i];
				FD_SET(client[i], &rfds);
			}
		}

		if (select(max_fd + 1, &rfds, NULL, NULL, NULL) < 0) {
			perror("select");
			return 1;
		}

		if (FD_ISSET(server_listen, &rfds)) {
			printf("Connect ready on serverlisten!\n");
			if ((fd = accept(server_listen, NULL, NULL)) < 0) {
				perror("accept");
				return 1;
			}
			if (fd > CLIENTSIZE) {
				printf("Too many clients!\n");
				close(fd);
			} else {
				make_nonblock(fd);
				if (client[fd] == -1) {
					client[fd] = fd;
				}
			}
		}
		for (int i = 0; i < CLIENTSIZE; i++) {
			if (i == server_listen) continue;
			if (FD_ISSET(i, &rfds)) {
				char buf[BUFSIZE] = {0};
				int retval = recv(i, buf, BUFSIZE, 0);
				if (retval <= 0) {
					printf("Logout!\n");
					client[i] = -1;
					close(i);
					continue;
				}
				printf("recv : %s", buf);
				for (int i = 0; i < strlen(buf); i++) {
					buf[i] = ch_char(buf[i]);
				}
				send(i, buf, strlen(buf), 0);
			}
		}
	}

	return 0;
}	
