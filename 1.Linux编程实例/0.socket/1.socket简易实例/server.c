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
#include <arpa/inet.h>

//需要传入参数个数以及参数列表
int main(int argc, char **argv) {
	if (argc != 2) {//参数个数不对的情况下
		fprintf(stderr, "Usage : %s port\n", argv[0]);//使用fprintf来输入到stderr中
		exit(1);//退出
	}
	//定义端口，以及监听
	int port, server_listen;
	port = atoi(argv[1]);//参数列表第一个数就是端口号

	//创建一个套接字，小于0表示出错了，否则返回门的id
	if ((server_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);//表示出错退出
	}

	printf("Socket create.\n");
	struct sockaddr_in server;//定义一个服务
	server.sin_family = AF_INET;//IP地址族
	server.sin_port = htons(port);//地址转换为字符串
	server.sin_addr.s_addr = INADDR_ANY;

	//绑定ip端口，强制类型转换为一个地址类型
	if (bind(server_listen, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("bind");
		exit(1);
	}
	
	printf("Socket bind.\n");
	//监听，表示等待别人连接，最大连接数设置为20
	if (listen(server_listen, 20) < 0) {
		perror("server");
		exit(1);
	}

	//假设每次都执行一次，单线程
	while (1) {
		int sockfd;
		printf("Socket accept.\n");
		//接受连接
		if ((sockfd = accept(server_listen, NULL, NULL)) < 0) {
			perror("accept");
			close(sockfd);//已经建立连接就关掉套接字
			continue;//一般不结束
		}
		//建立一个子进程,fork函数创建子进程，
		//变成两个进程，两个进程相互独立，可以做不同的事情
		pid_t pid;
		if ((pid = fork()) < 0) {
			perror("fork");
			continue;
		}
		if (pid == 0) {
			close(server_listen);
			char name[20] = {0};
			//接受信息，小于0表示出错了
			if (recv(sockfd, name, sizeof(name), 0) <= 0) {
				perror("recv");
				close(sockfd);//也是关闭套接字
			}
			printf("Socket received.\n");
			printf("Name = %s\n", name);//收到信息之后打印出来
			exit(0);
		}
		//对应父进程什么也不做
	}
	close(server_listen);
	return 0;
}
