/*************************************************************************
    > File Name: server.c
    > Author: ltw
    > Mail: 3245849061@qq.com 
    > Created Time: Thu 04 Jun 2020 06:10:16 PM CST
 ************************************************************************/

#include "../common/head.h"
#include "../common/color.h"
#include "../common/udp_server.h"
#include "../common/common.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage : %s port!\n", argv[0]);
		exit(1);
	}
	int port = atoi(argv[1]);
	int sockfd;
	struct sockaddr_in client;
	char msg[512] = {0};
	socklen_t len = sizeof(struct sockaddr_in);

	if ((sockfd = socket_create_udp(port)) < 0) {
		perror("socket_create_udp");
		exit(1);
	}
	
	recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&client, &len);
	printf("D: recv :%s \n", msg);

	if (connect(sockfd, (struct sockaddr *)&client, len) < 0) {
		perror("connect");
		exit(1);
	}

	send(sockfd, "Hello!", sizeof("Hello!"), 0);

	return 0;
}

