/*************************************************************************
    > File Name: server.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 22 Mar 2020 08:05:03 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>

void sig_chld(int signo) {
	int stat;
	wait(&stat);
}

/*服务端接受文件
void recv_file() {
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
	if ((fp = fopen(file_name, "a+")) < 0) {
		perror("fopen");
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
}*/

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage : %s port\n", argv[0]);
		exit(1);
	}
	int port, server_listen;
	port = atoi(argv[1]);

	if ((server_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	printf("Socket create.\n");
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_listen, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("bind");
		exit(1);
	}
	
	printf("Socket bind.\n");
	if (listen(server_listen, 20) < 0) {
		perror("server");
		exit(1);
	}

	while (1) {
		int sockfd;
		printf("Socket accept.\n");
		if ((sockfd = accept(server_listen, NULL, NULL)) < 0) {
			perror("accept");
			close(sockfd);
			continue;
		}
		pid_t pid;
		if ((pid = fork()) < 0) {
			perror("fork");
			continue;
		}
		if (pid == 0) {
			close(server_listen);
			char buf[1024] = {0};
			char file[1024] = {0};
			if (recv(sockfd, buf, sizeof(buf), 0) <= 0) {
				perror("recv");
				close(sockfd);
			}
			printf("Socket received.\n");
			strcpy(file, buf);
			FILE *fp = fopen(file, "r");
			int len = 0;
			while ((len = fread(buf, sizeof(char), 1024, fp)) > 0) {
				if (send(sockfd, buf, len, 0) < 0) {
					perror("send");
					break;
				}
				memset(buf, 0, sizeof(buf));
			}
			fclose(fp);
			exit(0);
		} else {
			signal(SIGCHLD, &sig_chld);
		}
	}
	close(server_listen);
	return 0;
}
