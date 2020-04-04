/*************************************************************************
    > File Name: client.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 22 Mar 2020 08:21:57 PM CST
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	int sockfd, port; 
	struct sockaddr_in server;
	if (argc != 4) {
		fprintf(stderr, "Usage : %s port file_name\n", argv[0]);
		exit(1);
	}
	port = atoi(argv[2]);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(argv[1]);
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	printf("Socket create.\n");

	if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("connect");
		exit(1);
	}
	while (1) {
		char file_name[1024];
		char buf[1024];
		memset(file_name, 0, sizeof(file_name));
		memset(buf, 0, sizeof(buf));
		strcpy(file_name, argv[3]);
		strcpy(buf, argv[3]);
		if (write(sockfd, buf, sizeof(buf)) < 0) {
			perror("write");
			exit(1);
		}
		FILE *fp;
		if ((fp = open(file_name, O_CREAT | O_RDWR, 755)) < 0) {
			perror("open");
			exit(1);
		}
		int len = 0;
		memset(buf, 0, sizeof(buf));
		while ((len = read(sockfd, buf, sizeof(buf))) > 0) {
			if (write(fp, buf, len) < 0) {
				perror("write");
				exit(1);
			}
			if (len < sizeof(buf)) break;
			memset(buf, 0, sizeof(buf));
		}
		close(fp);
	}
	close(sockfd);

	return 0;
}
