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
#define BUFSIZE 4096

struct Buffer{
	int fd;
	char buf[BUFSIZE];
	int flag;
	int sendindex;
	int recvindex;
};

struct Buffer *AllocBuffer() {
	struct Buffer *buffer = (struct Buffer *)malloc(sizeof(struct Buffer));
	if (buffer == NULL) {
		return NULL;
	}
	buffer->fd = -1;
	buffer->flag = buffer->recvindex = buffer->sendindex = 0;
	memset(buffer->buf, 0, BUFSIZE);
	return buffer;
}

void FreeBuffer(struct Buffer *buffer) {
	free(buffer);
}

char ch_char(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 32;
	}
	return c;
}

int RecvToBuffer(int fd, struct Buffer *buffer) {
	int recv_num;
	while (1) {
		char buf[BUFSIZE] = {0};
		recv_num = recv(fd, buf, sizeof(buf), 0);
		printf("last two char : %d and %d\n", buf[recv_num - 2], buf[recv_num - 1]);
		printf("recv : %s\n", buf);
		if (recv_num <= 0) {
			break;
		}
		for (int i = 0; i < recv_num; i++) {
			if (buffer->recvindex < sizeof(buffer->buf)){
				buffer->buf[buffer->recvindex++] = ch_char(buf[i]);
			}
			if (buffer->recvindex > 2 && buffer->buf[buffer->recvindex - 1] == 10 && buffer->buf[buffer->recvindex - 3] == 10) {
				buffer->flag = 1;
			}
		}
	}
	if (recv_num < 0) {
		if (errno == EAGAIN) {
			return 0;
		}
		return -1;
	} else if (recv_num == 0) {
		return 1;
	}
}

char SendFromBuffer(int fd, struct Buffer *buffer) {
	int send_num;
	while (buffer->sendindex < buffer->recvindex) {
		send_num = send(fd, buffer->buf + buffer->sendindex, buffer->recvindex - buffer->sendindex, 0);
		if (send_num < 0) {
			if (errno == EAGAIN) {
				return 0;
			}
			buffer->fd = -1;
			return -1;
		}
		buffer->sendindex += send_num;
	}
	if (buffer->sendindex == buffer->recvindex) {
		buffer->sendindex = buffer->recvindex = 0;
	}
	buffer->flag = 0;
	return 0;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr,"Usage %s port!\n", argv[0]);
		exit(1);
	}
	int server_listen, fd, max_fd;
	if ((server_listen = socket_create(atoi(argv[1]))) < 0) {
		perror("socket_create");
		exit(1);
	}
	struct Buffer *buffer[CLIENTSIZE];
	
	for (int i = 0; i < CLIENTSIZE; i++) {
		buffer[i] = AllocBuffer();
	}

	make_nonblock(server_listen);

	fd_set rfds, wfds, efds;
	max_fd = server_listen;
	buffer[server_listen]->fd = server_listen;

	while (1) {
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_ZERO(&efds);

		FD_SET(server_listen, &rfds);//加哪一个就关注哪一个，这个是关注读的

		for (int i = 0; i < CLIENTSIZE; i++) {
			if (buffer[i]->fd == server_listen) continue;//可以去掉这一行
			if (buffer[i]->fd > 0) {
				if (max_fd < buffer[i]->fd) max_fd = buffer[i]->fd;
				FD_SET(buffer[i]->fd, &rfds);
				if (buffer[i]->flag == 1) {
					FD_SET(buffer[i]->fd, &rfds);
				}
			}
		}

		if (select(max_fd + 1, &rfds, &wfds, NULL, NULL) < 0) {
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
				printf("You are login!\n");
				make_nonblock(fd);
				if (buffer[fd]->fd == -1) {
					buffer[fd]->fd = fd;
				}
			}
		}

		for (int i = 0; i <= max_fd; i++) {
			int retval = 0;
			if (i == server_listen || buffer[i]->fd < 0) continue;
			if (FD_ISSET(buffer[i]->fd, &rfds)) {
				printf("before : RECV");
				retval = RecvToBuffer(i, buffer[i]);
				printf("retval = %d\n", retval);
			}
			if (retval == 0 && FD_ISSET(i, &wfds)) {
				retval = SendFromBuffer(i, buffer[i]);
			}
			if (retval) {
				printf("Logout !\n");
				buffer[i]->fd = -1;
				close(i);
			}
		}
	}

	return 0;
}	
