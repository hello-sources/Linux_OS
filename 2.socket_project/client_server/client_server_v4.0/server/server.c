/*************************************************************************
    > File Name: server.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Mon 30 Mar 2020 01:12:54 AM CST
 ************************************************************************/

#include "head.h"
#include "tcp_server.h"
#define MAXCLIENT 512

struct Client {
	int flag;
	int fd;
	pthread_t tid;
};

struct Client *client;

int find_sub() {
	for (int i = 0; i < MAXCLIENT; i++) {
		if (client[i].flag == 0) {
			return i;
		}
	}
	return -1;
}

void chstr(char *str) {
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] >= 'a' && str[i] <= 'z') {
			str[i] -= 32;
		}
	}
}


void *work(void *arg) {
	int *sub = (int *)arg;
	int fd = client[*sub].fd;
	if (send(fd, "You are here.", sizeof("You are here."), 0) < 0) {
		perror("send");
		close(fd);
		return NULL;
	}
	while (1) {
		char msg[512] = {0};
		if (recv(fd, msg, sizeof(msg), 0) <= 0){
			perror("recv");
			close(fd);
			client[*sub].flag = 0;
			break;
		}
		printf("recv : %s\n", msg);
		chstr(msg);
		if (send(fd, msg, strlen(msg), 0) < 0) {
			perror("send");
			break;
		}
		printf("Success in ECHO!\n");
	}
	close(fd);
	client[*sub].flag = 0;
	return NULL;
}


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
		
	pthread_t tid;
	client = (struct Client *)malloc(sizeof(struct Client) * MAXCLIENT);
	
	while (1) {
		int sub, fd;
		if ((fd = accept(server_listen, NULL, NULL)) < 0) {
			perror("accept");
		}
		printf("Mew Client Login!\n");
		if ((sub = find_sub()) < 0) {
			fprintf(stderr, "Full!\n");
			close(fd);
			continue;
		}
		client[sub].flag = 1;
		client[sub].fd = fd;
		pthread_create(&client[sub].tid, NULL, work, (void *)&sub);
	}

	return 0;
}
