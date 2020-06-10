/*************************************************************************
	> File Name: udp_epoll.h
	> Author: ltw
	> Mail: 3245849061@qq.com
	> Github: https://github.com/hello-sources
	> Created Time: Mon 08 Jun 2020 09:30:23 PM CST
 ************************************************************************/


#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H
#include "datatype.h"
void add_event(int epollfd, int fd, int events);
void del_event(int epollfd, int fd, int events);
//int udp_connect(int epollfd, struct sockaddr_in *serveraddr);
int udp_accept(int epollfd, int fd, struct User *user);
void add_to_sub_reactor(struct User *user);
#endif
