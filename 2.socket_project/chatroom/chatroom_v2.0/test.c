/*************************************************************************
    > File Name: test.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 29 Mar 2020 03:58:51 PM CST
 ************************************************************************/

#include "./common/head.h"
#include "./common/common.h"

char ans[50] = {0};

int main() {
	//get_value("./a.conf", "SERVERIP");
	printf("%s\n", get_value("./a.conf", "SERVERIP"));
	return 0;
}
