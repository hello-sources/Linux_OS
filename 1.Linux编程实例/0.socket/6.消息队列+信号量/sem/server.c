/*************************************************************************
    > File Name: server.c
    > Author: ltw
    > Mail: 3245849061@qq.com 
    > Created Time: Sun 10 May 2020 08:00:09 PM CST
 ************************************************************************/

#include "./common/color.h"
#include "./common/common.h"
#include "./common/tcp_server.h"
#include "./common/head.h"
#include <sys/ipc.h>
#include <sys/sem.h>

#define MAX 10
#define MAXSEQ 100

union semnum {
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
};

static int total = 0;
static int sem_id = 0;
static int set_semvalue();
static void del_semvalue();
static int semaphore_p();
static int semaphore_v();

void *work(void *arg) {
	int new_fd = *(int *)arg;
	semaphore_p();
	char msg[512] = "You can say!\n";
	send(new_fd, msg, strlen(msg), 0);
	while (1) {
		memset(msg, 0, sizeof(msg));
		int rnum = recv(new_fd, msg, sizeof(msg), 0);
		if (rnum <= 0) {
			total--;
			semaphore_v();
			close(new_fd);
			return 0;
		}
		send(new_fd, msg, strlen(msg), 0);
	}
	return NULL;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("error!\n");
		exit(1);
	}
	
	if ((sem_id = semget((key_t)8731, 1, 0666 | IPC_CREAT)) < 0) {
		perror("semget");
		exit(1);
	}

	if (!set_semvalue()) {
		perror("set_semvalue");
		exit(1);
	}
	
	int server_listen, fd;
	if ((server_listen = socket_create(atoi(argv[1]))) < 0) {
		perror("socket_create!");
		exit(1);
	}
	while (1) {
		if ((fd = accept(server_listen, NULL, NULL)) < 0) {
			perror("accept");
			exit(1);
		}
		total++;
		printf("The %dth client!\n", total);
		send(fd, "You are Here!\n", sizeof("You are Here!\n"), 0);
		if (total > MAXSEQ) {

		}
		pthread_t tid;
		pthread_create(&tid, NULL, work, (void *)&fd);
	}
	sleep(1000);
}

static int set_semvalue() {
	union semnum sem_union;
	sem_union.val = MAX;

	if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
		return 0;
	}
	return 1;
}

static void del_semvalue() {
	union semnum sem_union;
	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
		perror("semctl_RMID");
	}
}

static int semaphore_p() {
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;

	if (semop(sem_id, &sem_b, 1) == -1) {
		perror("semop_p");
		return 0;
	}
	return 1;
}

static int semaphore_v() {
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;

	if (semop(sem_id, &sem_b, 1) == -1) {
		perror("semop_v");
		return 0;
	}
	return 1;
}
