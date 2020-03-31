/*************************************************************************
    > File Name: server.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 29 Mar 2020 04:12:35 PM CST
 ************************************************************************/

#include "../common/head.h"
#include "../common/common.h"
#include "../common/tcp_server.h"
#include "../common/chatroom.h"
#include "../common/color.h"

char *conf = "./server.conf";

struct User {
	char name[20];
	int online;
	pthread_t tid;
	int fd;
};

struct User *client;

void *work(void *arg) {
	printf("client login!\n");
	int sub = *(int *)arg;
	int client_fd = client[sub].fd;
	struct RecvMsg rmsg;
	printf(GREEN"Login :"NONE" : %s \n",rmsg.msg.from);
	while (1) {
		rmsg = chat_recv(client_fd);
		if (rmsg.retval < 0) {
			printf(PINK"Logout :"NONE" %s \n", client[sub].name);
			close(client_fd);
			client[sub].online = 0;
			return NULL;
		}
		printf(BLUE"%s"NONE" : %s \n", rmsg.msg.from, rmsg.msg.message);
	}
	return NULL;
}

int find_sub() {
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (!client[i].online) return i;
	}
	return -1;
}

bool check_online(char *name) {
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (client[i].online && !strcmp(name, client[i].name)) {
			printf(YELLOW"D"NONE" : %s is online!\n", name);
			return true;
		}
	} 
	return false;
}


int main() {
	int port, server_listen, fd;
	struct RecvMsg recvmsg;
	struct Msg msg;
	port = atoi(get_value(conf, "SERVER_PORT"));
	client = (struct User *)calloc(MAX_CLIENT, sizeof(struct User));
	if ((server_listen = socket_create(port)) < 0) {
		perror("socket_create");
		return 1;
	}
	while (1) {
		if ((fd = accept(server_listen, NULL, NULL)) < 0) {
			perror("accept");
			continue;
		}
		recvmsg = chat_recv(fd);
		if (recvmsg.retval < 0) {
			close(fd);
			continue;
		}
		if (check_online(recvmsg.msg.from)) {
			msg.flag = 3;
			strcpy(msg.message, "You Have Already Login!");
			chat_send(msg, fd);
			close(fd);
			continue;
		} 
		msg.flag = 2;
		strcpy(msg.message, "Welcome to this chat room!");
		chat_send(msg, fd);

		int sub;
		sub = find_sub();
		client[sub].online = 1;
		client[sub].fd = fd;
		strcpy(client[sub].name, recvmsg.msg.from);
		pthread_create(&client[sub].tid, NULL, work, (void *)&sub);
	}
	printf("%d\n", port);
	return 0;
}

