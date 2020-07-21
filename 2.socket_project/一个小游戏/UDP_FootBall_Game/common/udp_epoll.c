/************************************************************
    File Name : udp_epoll.c
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/04 19:07:10
************************************************************/

#include "./common.h"
#include "./head.h"
#include "./udp_client.h"
#include "./udp_server.h"

#define MAX 50

extern int port;
extern struct User *rteam;
extern struct User *bteam;
extern int repollfd, bepollfd;
extern struct Map court;

void add_event(int epollfd, int fd, int events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    DBG(YELLOW "EPOLL" NONE " : After Epoll Add.\n");
    return;
}

void add_event_ptr(int epollfd, int fd, int events, struct User *user) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = (void *)user;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    DBG(GREEN "Sub Thread" NONE " : After Epoll Add %s.\n", user->name);
    return;
}

void del_event(int epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
    return;
}

int udp_connect(int epollfd, struct sockaddr_in *serveraddr) {
    int sockfd;
    if ((sockfd = socket_create_udp(port)) < 0) {
        perror("socket_udp");
        return -1;
    }
    if ((connect(sockfd, (struct sockaddr *)serveraddr,
                 sizeof(struct sockaddr))) < 0) {
        perror("connect");
        return -1;
    }
    DBG(GREEN "INFO" NONE " : After connect.\n");
    return sockfd;
}

// 检查全场是否有同名用户已经登录 有则返回1 否则0
int check_online(struct LogRequest *request) {
    for (int i = 0; i < MAX; ++i) {
        if (rteam[i].online && !strcmp(rteam[i].name, request->name)) return 1;
        if (bteam[i].online && !strcmp(bteam[i].name, request->name)) return 1;
    }
    return 0;
}

int udp_accept(int epollfd, int fd, struct User *user) {
    struct sockaddr_in client;
    int new_fd, ret;
    struct LogRequest request;
    struct LogResponse response;

    bzero(&request, sizeof(request));
    bzero(&response, sizeof(response));

    socklen_t len = sizeof(struct sockaddr_in);
    ret = recvfrom(fd, (void *)&request, sizeof(request), 0,
                   (struct sockaddr *)&client, &len);
    if (ret != sizeof(request)) {
        response.type = 1;  // Unsuccessful
        strcpy(response.msg, "Login failed. Network error!");
        sendto(fd, (void *)&response, sizeof(response), 0,
               (struct sockaddr *)&client, len);

        return -1;
    }

    // 防止同名用户重复上线
    if (check_online(&request)) {
        response.type = 1;
        strcpy(response.msg, "You are already online!");
        sendto(fd, (void *)&response, sizeof(response), 0,
               (struct sockaddr *)&client, len);
        return -1;
    }

    response.type = 0;
    strcpy(response.msg, "Login success. Enjoy yourself.");
    sendto(fd, (void *)&response, sizeof(response), 0,
           (struct sockaddr *)&client, len);

    if (request.team) {  // Team is blue
        DBG(GREEN "INFO" NONE " : " BLUE " %s on %s:%d login! (%s)\n" NONE,
            request.name, inet_ntoa(client.sin_addr), ntohs(client.sin_port),
            request.msg);
    } else {
        DBG(GREEN "INFO" NONE " : " RED " %s on %s:%d login! (%s)\n" NONE,
            request.name, inet_ntoa(client.sin_addr), ntohs(client.sin_port),
            request.msg);
    }
    strcpy(user->name, request.name);
    user->team = request.team;
    new_fd = udp_connect(epollfd, &client);
    user->fd = new_fd;
    user->loc.x = rand() % (court.width + 4);
    user->loc.y = rand() % (court.height + 2);
    return new_fd;
}

int find_sub(struct User *team) {
    for (int i = 0; i < MAX; ++i) {
        if (!team[i].online) return i;
    }
    return -1;
}

void add_to_sub_reactor(struct User *user) {
    struct User *team = (user->team ? bteam : rteam);
    DBG(YELLOW "Main Thread : " NONE "Add to sub_reactor\n");
    int sub = find_sub(team);
    team[sub] = *user;
    team[sub].online = 1;
    team[sub].flag = 10;
    DBG(L_RED "sub = %d, name = %s" NONE "\n", sub, team[sub].name);
    if (user->team) {  // Blue Team
        add_event_ptr(bepollfd, team[sub].fd, EPOLLIN | EPOLLET, &team[sub]);
    } else {
        add_event_ptr(repollfd, team[sub].fd, EPOLLIN | EPOLLET, &team[sub]);
    }
    return;
}