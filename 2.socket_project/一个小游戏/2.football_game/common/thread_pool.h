/*************************************************************************
	> File Name: thread_pool.h
	> Author: ltw
	> Mail: 3245849061@qq.com
	> Github: https://github.com/hello-sources
	> Created Time: Tue 09 Jun 2020 06:14:36 PM CST
 ************************************************************************/

#ifndef _THREAD_POLL_H
#define _THREAD_POLL_H
#include "head.h"
#define MAXTASK 100
#define MAXTHREAD 10

struct task_queue{
    int sum;
    int epollfd;
    struct User **team;
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd);
void task_queue_push(struct task_queue *taskQueue, struct User *user);
struct User *task_queue_pop(struct task_queue *taskQueue);
void *thread_run(void *arg);

#endif
