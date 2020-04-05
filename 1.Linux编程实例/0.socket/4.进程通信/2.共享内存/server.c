/*************************************************************************
    > File Name: server.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 04 Apr 2020 08:05:11 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>

struct Shm {
	pid_t pid;
	char buf[1024];
};

struct Shm *share_memory = NULL;

void print(int sig) {
	printf("<parent> : %s\n", share_memory->buf);
}

int main() {
	int shmid;
	key_t key = ftok(".", 198);
	
	if ((shmid = shmget(key, sizeof(struct Shm), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		return 1;
	}
	printf("After shmget!\n");
	if ((share_memory = (struct Shm *)shmat(shmid, NULL, 0)) == NULL) {
		perror("shmat");
		return 1;
	}
	printf("After shmat!\n");
	share_memory->pid = getpid();
	while (1) {
		signal(SIGUSR2, print);
		pause();
	}
	return 0;
}
