/************************************************************
    File Name : tcp_client.h
    Author: Ginakira
    Mail: ginakira@outlook.com
    Github: https://github.com/Ginakira
    Created Time: 2020/03/28 14:34:54
************************************************************/

#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H
int socket_connect(char *host, int port);
int socket_connect_timeout(char *host, int port, long timeout);
#endif