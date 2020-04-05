/*************************************************************************
    > File Name: client.c
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
#include <string.h>
#include <pthread.h>

struct Shm {
	pthread_cond_t cond;
	pthread_mutex_t mutex;
//	pid_t pid;
	char buf[1024];
};

struct Shm *share_memory = NULL;

int main() {
	int shmid;
	key_t key = ftok(".", 198);
	if ((shmid = shmget(key, sizeof(struct Shm), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		return 1;
	}

	if ((share_memory = (struct Shm *)shmat(shmid, NULL, 0)) == NULL) {
		perror("shmat");
		return 1;
	}

	while (1) {
		printf("Before mutex lock!\n");
		usleep(1000);
		pthread_mutex_lock(&share_memory->mutex);
		printf("After mutex lock!\n");
		printf("<client> : ");
		scanf("%[^\n]s", share_memory->buf);
		getchar();
		pthread_cond_signal(&share_memory->cond);
		//sleep(1);
		pthread_mutex_unlock(&share_memory->mutex);
		//pthread_cond_signal(&share_memory->cond);
	}
	return 0;
}
