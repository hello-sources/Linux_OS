/*************************************************************************
    > File Name: test.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Tue 31 Mar 2020 11:12:55 AM CST
 ************************************************************************/

#include "../common/head.h"
#include "../common/common.h"
#include "../common/tcp_server.h"

char *conf = "./server.conf";

int main() {
	int port;
	port = atoi(get_value(conf, "SERVER_PORT"));
	printf("%d\n", port);
	return 0;
}
