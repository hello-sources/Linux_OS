/************************************************************
    File Name : thread_pool.h
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/09 18:14:33
************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include "head.h"

#define MAXTASK 100
#define MAXTHREAD 10

struct task_queue {
    int sum;
    int epollfd;  // 对应的从反应堆
    int head;
    int tail;
    struct User **team;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

void send_all(struct FootBallMsg msg);
void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd);
void task_queue_push(struct task_queue *taskQueue, struct User *user);
struct User *task_queue_pop(struct task_queue *taskQueue);
void *thread_run(void *arg);

#endif