/************************************************************
    File Name : udp_epoll.h
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/06/04 19:24:03
************************************************************/

#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H

#include "datatype.h"
void add_event(int epollfd, int fd, int events);
void del_event(int epollfd, int fd);
int udp_connect(int epollfd, struct sockaddr_in *serveraddr);
int udp_accept(int epollfd, int fd, struct User *user);
void add_to_sub_reactor(struct User *user);
#endif