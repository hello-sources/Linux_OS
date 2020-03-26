/*************************************************************************
    > File Name: pthread.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Thu 26 Mar 2020 07:50:13 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

struct Msg {
	int age;
	char name[20];
};

void *print(void *arg) {
	struct Msg *tmp;
	tmp = (struct Msg *)arg;

	printf("name = %s, age = %d\n", tmp->name, tmp->age);
	//return NULL;
	int retval = 3;
	//exit(3);
	pthread_exit((void *)&retval);
}


int main() {
	void *status;
	pthread_t tid;
	struct Msg msg;
	msg.age = 18;
	strcpy(msg.name, "ltw");
	pthread_create(&tid, NULL, print, (void *)&msg);
	//sleep(1);
	//pthread_join(tid, NULL);
	pthread_join(tid, &status);
	printf("%d\n", *(int *)status);
	return 0;
}
