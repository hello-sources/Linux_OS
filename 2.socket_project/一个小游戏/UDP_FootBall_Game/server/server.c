/************************************************************
    File Name : server.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/02 18:13:42
************************************************************/

#include "../common/game.h"
#include "../common/head.h"
#include "../common/heart_beat.h"
#include "../common/server_exit.h"
#include "../common/server_re_draw.h"
#include "../common/show_data_stream.h"
#include "../common/sub_reactor.h"
#include "../common/thread_pool.h"
#include "../common/udp_epoll.h"
#include "../common/udp_server.h"

char *conf = "./server.conf";

struct User *rteam;
struct User *bteam;
int port = 0;
int repollfd, bepollfd;
struct Bpoint ball;
struct BallStatus ball_status;
struct Score score;

// struct Map court;

int main(int argc, char **argv) {
    int opt, listener, epoll_fd;
    pthread_t draw_t, red_t, blue_t, heart_t;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
                exit(1);
        }
    }
    argc -= (optind - 1);
    argv += (optind - 1);

    if (argc > 1) {
        fprintf(stderr, "Usage: %s [-p port]\n", argv[0]);
        exit(1);
    }

    if (!port) port = atoi(get_value(conf, "PORT"));
    court.width = atoi(get_value(conf, "COLS"));
    court.height = atoi(get_value(conf, "LINES"));
    court.start.x = 3;
    court.start.y = 2;
    court.gate_height = 9;

    ball.x = court.width / 2;
    ball.y = court.height / 2;

    rteam = (struct User *)calloc(MAX, sizeof(struct User));
    bteam = (struct User *)calloc(MAX, sizeof(struct User));

    if ((listener = socket_create_udp(port)) < 0) {
        perror("socket_create_udp");
        exit(1);
    }

    DBG(GREEN "INFO" NONE " : Server start on Port %d\n", port);

#ifndef _D
    pthread_create(&draw_t, NULL, draw, NULL);
#endif

    epoll_fd = epoll_create(MAX * 2);
    repollfd = epoll_create(MAX);
    bepollfd = epoll_create(MAX);

    if (epoll_fd < 0 || repollfd < 0 || bepollfd < 0) {
        perror("epoll_create");
        exit(1);
    }

    struct task_queue redQueue;
    struct task_queue blueQueue;

    task_queue_init(&redQueue, MAX, repollfd);
    task_queue_init(&blueQueue, MAX, bepollfd);

    pthread_create(&red_t, NULL, sub_reactor, (void *)&redQueue);
    pthread_create(&blue_t, NULL, sub_reactor, (void *)&blueQueue);
    pthread_create(&heart_t, NULL, heart_beat, NULL);

    signal(SIGINT, server_exit);

    struct epoll_event ev, events[MAX * 2];
    ev.events = EPOLLIN;
    ev.data.fd = listener;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener, &ev);
    // struct sockaddr_in client;
    // socklen_t len = sizeof(client);

    // 定时重绘
    signal(SIGALRM, re_draw);

    struct itimerval itimer;
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 100000;
    itimer.it_value.tv_sec = 2;
    itimer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itimer, NULL);

    Show_Message(, , "Waiting for login...", 1);
    while (1) {
        DBG(YELLOW "Main Thread" NONE " : Before epoll_wait\n");
        int nfds = epoll_wait(epoll_fd, events, MAX * 2, -1);
        DBG(YELLOW "Main Thread" NONE " : After epoll_wait\n");

        for (int i = 0; i < nfds; ++i) {
            struct User user;
            bzero(&user, sizeof(user));
            char buff[512] = {0};
            DBG(YELLOW "EPOLL" NONE " : Doing with %dth fd\n", i);
            if (events[i].data.fd == listener) {
                int new_fd = udp_accept(epoll_fd, listener, &user);
                if (new_fd > 0) {
                    sprintf(buff, "%s login the Game.", user.name);
                    DBG(YELLOW "Main Thread" NONE
                               " : Add %s to %s sub_reactor.\n",
                        user.name, (user.team ? "BLUE" : "RED"));
                    add_to_sub_reactor(&user);
                    show_data_stream('l');
                    Show_Message(, , buff, 1);

                    char buff[100] = {0};
                    sprintf(buff, "[INIT] ballx=%d bally=%d", (int)ball.x,
                            (int)ball.y);
                    Show_Message(, , buff, 1);
                }
            } else {
                recv(events[i].data.fd, buff, sizeof(buff), 0);
                printf(PINK "RECV" NONE " : %s\n", buff);
            }
            // char info[1024] = {0};
            // w_gotoxy_puts(Message, 1, 2, info);
        }
    }
    return 0;
}
