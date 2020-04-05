/*************************************************************************
    > File Name: shm1.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 04 Apr 2020 07:08:59 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

struct Shm {
	int now;
	int sum;
	pthread_mutex_t mutex;
}; 

struct Shm  *share_memory = NULL;

void do_add(int max, int x) {
	while (1) {
		pthread_mutex_lock(&share_memory->mutex);
		printf("<%d> : %d %d\n", x, share_memory->now, share_memory->sum);
		if (share_memory->now > max) {
			pthread_mutex_unlock(&share_memory->mutex);
			break;
		}
		share_memory->sum += share_memory->now;
		share_memory->now++;
		pthread_mutex_unlock(&share_memory->mutex);
	}
}

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "Usage : %s max ins\n", argv[0]);
		exit(1);
	}
	int max, ins;
	max = atoi(argv[1]);
	ins = atoi(argv[2]);
	pid_t pid;
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

	memset(share_memory, 0, sizeof(struct Shm));
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

	pthread_mutex_init(&share_memory->mutex, &attr);
	
	int x;

	for(int i = 0; i < ins; i++) {
        if ((pid = fork()) < 0) {
            perror("fork");
            return 1;
         }
         if (pid == 0) {
             x = i;
             break;
         }
     }
    if (pid == 0) {
        do_add(max, x);
        printf("<%d> exit!\n", x);
        exit(0);
    }
    while (ins) {
        wait(NULL);
        ins--;
    }
	printf("Ans = %d \n", share_memory->sum);
	return 0;
}
