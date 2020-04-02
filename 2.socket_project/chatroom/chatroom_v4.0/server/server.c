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
int sum = 0;

void send_all(struct Msg msg) {
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (client[i].online) {
			chat_send(msg, client[i].fd);
		}
	}
}

int check_name(char *name) {
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (client[i].online && !strcmp(client[i].name, name)) {
			return i;
		}
	}
	return -1;
}

int check_sum() {
	int ans = 0;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (!client[i].online) continue;
		ans++;
	}
	return ans;

}

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
			sum--;
			return NULL;
		}
		printf(BLUE"%s"NONE" : %s \n", rmsg.msg.from, rmsg.msg.message);
		if (rmsg.msg.flag == 0) {
			send_all(rmsg.msg);
		} else if (rmsg.msg.flag == 1) {
			if (rmsg.msg.message[0] == '@') {
				char to[20] = {0};
				int i = 1;
				for (; i <= 20; i++) {
					if (rmsg.msg.message[i] == ' ') {
						break;
					}
				}
				strncpy(to, rmsg.msg.message + 1, i - 1);
				int ind;
				if ((ind = check_name(to)) > 0) {
					fprintf(rmsg.msg.message, "%s is not online.", to);
					chat_send(rmsg.msg, client_fd);
					continue;
				}
				chat_send(rmsg.msg, client[ind].fd);
			}
		}
		if (rmsg.msg.message[0] == '#') {
			if (rmsg.msg.message[1] = '1') {
				printf(GREEN"%d"NONE"\n", sum);
				fprintf(rmsg.msg.message, "%d 个人\n", sum);
				for (int i = 0; i < MAX_CLIENT; i++) {
					if (!client[i].online) continue;
					char name[20];
					strcpy(name, client[i].name);
					printf(L_RED"%s"NONE"\n", client[i].name);
					fprintf(rmsg.msg.message, "%s ", client[i].name);
				}
				chat_send(rmsg.msg, client_fd);
			}
		}
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
		sum++;
		sub = find_sub();
		client[sub].online = 1;
		client[sub].fd = fd;
		strcpy(client[sub].name, recvmsg.msg.from);
		pthread_create(&client[sub].tid, NULL, work, (void *)&sub);
	}
	printf("%d\n", port);
	return 0;
}
