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

//同样传入参数个数，参数列表
int main(int argc, char **argv) {
	int sockfd, port; //定义端口以及套接字描述符
	struct sockaddr_in server;//连接server的结构体
	if (argc != 3) {//判断参数，本身是一个，IP地址一个，端口号一个
		fprintf(stderr, "Usage : %s ip port\n", argv[0]);
		exit(1);
	}
	port = atoi(argv[2]);

	server.sin_family = AF_INET;//地址族
	server.sin_port = htons(port);//转换为字符串
	server.sin_addr.s_addr = inet_addr(argv[1]);//IP地址字符串转换为网络字节序
	
	//套接字的创建，就是门的创建，以流的形式
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	printf("Socket create.\n");

	//连接服务端
	if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("connect");
		exit(1);
	}

	//向server发送信息
	if (send(sockfd, "ltw", sizeof("ltw"), 0) < 0) {
		perror("send");
		exit(1);
	}
	//存在问题就是对于一个进程进来一直占用的时候，别的连接不上
	close(sockfd);

	return 0;
}
