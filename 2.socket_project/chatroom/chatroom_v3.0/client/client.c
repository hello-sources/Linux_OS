/*************************************************************************
    > File Name: client.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 29 Mar 2020 06:16:06 PM CST
 ************************************************************************/

#include "../common/chatroom.h"
#include "../common/tcp_client.h"
#include "../common/common.h"
#include "../common/color.h"

char *conf = "./client.conf";
int sockfd;

char logfile[50] = {0};
void *logout(int signalnum) {
	close(sockfd);
	exit(1);
	printf("recv a signal");

}


int main() {
	int port;
	struct Msg msg;
	char ip[20] = {0};
	port = atoi(get_value(conf, "SERVER_PORT"));
	strcpy(ip, get_value(conf, "SERVER_IP"));
	strcpy(logfile, get_value(conf, "LOG_FILE"));
	printf("ip = %s, port = %d.\n", ip, port);

	if ((sockfd = socket_connect(ip, port)) < 0) {
		perror("socket_connect");
		return 1;
	}
	strcpy(msg.from, get_value(conf, "MY_NAME"));
	msg.flag = 2;
	if (chat_send(msg, sockfd) < 0) {
		return 2;
	}

	struct RecvMsg rmsg = chat_recv(sockfd);
	
	if (rmsg.retval < 0) {
		fprintf(stderr, "Error!\n");
		return 1;
	}

	printf(GREEN"Server"NONE": %s\n", rmsg.msg.message);

	if (rmsg.msg.flag == 3) {
		close(sockfd);
	}
	
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("fork");
	}
	if (pid == 0) {
		sleep(2);
		//signal(SIGINT, logout);
		system("clear");
		while (1) {
			printf(L_PINK"Please Input Message:"NONE"\n");
			scanf("%[^\n]s", msg.message);
			getchar();
			msg.flag = 0;
			if (msg.message[0] == '@') {
				msg.flag = 1;
			}
			chat_send(msg, sockfd);
			memset(msg.message, 0, sizeof(msg.message));
			//system("clear");
		}
	} else {
		//FILE *log_fp = fopen(logfile, "w");
		freopen(logfile, "a+", stdout);
		while (1) {
			rmsg = chat_recv(sockfd);
			if (rmsg.msg.flag == 0) {
				//fprintf(log_fp, L_BLUE"%s "NONE": %s\n", rmsg.msg.from, rmsg.msg.message);
				printf("%s : %s\n", rmsg.msg.from, rmsg.msg.message);
				//fflush(stdout);
			//fflush(log_fp);
			} else if (rmsg.msg.flag == 2) {
				printf(YELLOW"通知信息: "NONE" %s\n", rmsg.msg.message);
			} else if (rmsg.msg.flag == 1) {
				printf("私聊\n");
			} else {
				printf("Error!\n");
			}
		}
		fflush(stdout);
		wait(NULL);
		close(sockfd);
	}

	return 0;
}
