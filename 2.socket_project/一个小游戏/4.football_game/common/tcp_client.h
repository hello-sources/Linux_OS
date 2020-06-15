/************************************************************
    File Name : tcp_client.h
    Author: ltw
    Mail: 3245849061@qq.com
    Github: https://github.com/hello-sources
    Created Time: 2020/03/28 14:34:54
************************************************************/

#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H
int socket_connect(char *host, int port);
int socket_connect_timeout(char *host, int port, long timeout);
#endif
