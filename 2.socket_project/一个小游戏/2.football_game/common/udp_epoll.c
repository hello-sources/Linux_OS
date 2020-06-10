/*************************************************************************
	> File Name: udp_epoll.c
	> Author: ltw
	> Mail: 3245849061@qq.com
	> Github: https://github.com/hello-sources
	> Created Time: Mon 08 Jun 2020 09:29:48 PM CST
 ************************************************************************/

#include "./common.h"
#include "./udp_client.h"
#include "./udp_server.h"
#include "./head.h"
#define MAX 50
extern int port;

extern struct User *rteam;
extern struct User *bteam;
extern int repollfd, bepollfd;

void add_event(int epollfd, int fd, int events){
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    DBG(YELLOW"EPOLL"NONE" : After Epoll Add.\n");
    return ;
}

void add_event_ptr(int epollfd, int fd, int events, struct User *user) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = (void *)user;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    DBG(GREEN"Sub Thread"NONE" : After Epoll Add %s.\n", user->name);
    return ;
}

void del_event(int epollfd, int fd, int events) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
    return ;
}

int udp_connect(int epollfd, struct sockaddr_in *serveraddr) {
    int sockfd;
    if ((sockfd = socket_create_udp(port)) < 0) {
        perror("socket_udp");
        return -1;
    }
    if ((connect(sockfd, (struct sockaddr *)serveraddr, sizeof(struct sockaddr))) < 0) {
        perror("connect");
        return -1;
    }
    DBG(GREEN"INFO"NONE" : After connect.\n");
    int ret = send(sockfd, "Login!", sizeof("Login!"), 0);
    DBG(RED"RET = %d\n"NONE, ret);
    //add_event(epollfd, sockfd, EPOLLIN);
    return sockfd;
}

int udp_accept(int epollfd, int fd, struct User *user) {
    struct sockaddr_in client;
    int new_fd, ret;
    struct LogRequest request;
    struct LogResponse response;
    bzero(&request, sizeof(request));
    bzero(&response, sizeof(response));

    socklen_t len  = sizeof(struct sockaddr_in);
    ret = recvfrom(fd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&client, &len);
    if (ret != sizeof(request)) {
        response.type = 1;
        strcpy(response.msg, "Login failed.");
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;
    }
    response.type = 0;
    strcpy(response.msg, "Login success. Enjoy yourself.");
    sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);

    if (request.team)
        DBG(GREEN"INFO"NONE" : "BLUE" %s on %s:%d login! (%s)\n"NONE, request.name, inet_ntoa(client.sin_addr), ntohs(client.sin_port), request.msg);
    else
        DBG(GREEN"INFO"NONE" : "RED" %s on %s:%d login! (%s)\n"NONE, request.name, inet_ntoa(client.sin_addr), ntohs(client.sin_port), request.msg);
    strcpy(user->name, request.name);
    user->team = request.team;
    new_fd = udp_connect(epollfd, &client);
    user->fd = new_fd;
    return new_fd;
}

int find_sub(struct User *team) {
    for (int i = 0; i < MAX; i++) {
        if (!team[i].online) return i;
    }
    return -1;
}

void add_to_sub_reactor(struct User *user) {
    struct User *team = (user->team ? bteam : rteam);
    DBG(YELLOW"Main Thread : "NONE"Add to sub_reactor\n");
    int sub = find_sub(team);
    team[sub] = *user;
    team[sub].online = 1;
    team[sub].flag = 10;
    DBG(L_RED"sub = %d, name = %s\n"NONE, sub, team[sub].name);
    if (user->team)
        add_event_ptr(bepollfd, team[sub].fd, EPOLLIN | EPOLLET, &team[sub]);
    else 
        add_event_ptr(repollfd, team[sub].fd, EPOLLIN | EPOLLET, &team[sub]);
    return ;
}
