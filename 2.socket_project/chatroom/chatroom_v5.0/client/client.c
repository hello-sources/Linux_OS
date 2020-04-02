/*************************************************************************
	> File Name: client.c
	> Author: suyelu
	> Mail: suyelu@haizeix.com
	> Created Time: 日  3/29 18:16:00 2020
 ************************************************************************/

#include "../common/chatroom.h"
#include "../common/tcp_client.h"
#include "../common/common.h"
#include "../common/color.h"

char *conf = "./client.conf";
int sockfd;

char logfile[50] = {0};
void logout(int signalnum) {
    close(sockfd);
    printf("您已退出.\n");
    exit(1);
}


int main() {
    int port;
    struct Msg msg;
    char ip[20] = {0};
    port = atoi(get_value(conf, "SERVER_PORT"));
    strcpy(ip, get_value(conf, "SERVER_IP"));
    strcpy(logfile, get_value(conf, "LOG_FILE"));
    

    if ((sockfd = socket_connect(ip, port)) < 0) {
        perror("socket_connect");
        return 1;
    }
    //strcpy(msg.from, get_value(conf, "MY_NAME"));
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    
    strcpy(msg.from, pwd->pw_name);
    msg.flag = 2;
    if (chat_send(msg, sockfd) < 0) {
        return 2;
    }
    
    struct RecvMsg rmsg = chat_recv(sockfd);
    
    if (rmsg.retval < 0) {
        fprintf(stderr, "Error!\n");
        return 1;
    }


    if (rmsg.msg.flag == 3) {
        close(sockfd);
        return 1;
    }

    signal(SIGINT, logout);
    pid_t pid;
    if ((pid = fork()) < 0){
        perror("fork");
    }
    if (pid == 0) {
        sleep(2);
        char c = 'a';
        while (c != EOF) {
            system("clear");
            printf(L_PINK"Please Input Message:"NONE"\n");
            memset(msg.message, 0, sizeof(msg.message));
            scanf("%[^\n]s", msg.message);
            c = getchar();
            msg.flag = 0;
            if (msg.message[0] == '@') {
                msg.flag = 1;
            } else if (msg.message[0] == '#') {
                msg.flag = 4;
            }
            if (!strlen(msg.message)) continue;
            chat_send(msg, sockfd);
        }
        close(sockfd);
    } else {
        freopen(logfile, "w", stdout);
        printf(L_GREEN"Server "NONE": %s\n", rmsg.msg.message);
        fflush(stdout);
        while (1) {
            rmsg = chat_recv(sockfd);
            if (rmsg.retval < 0) {
                printf("Error in Server!\n");
                break;
            }
            if (rmsg.msg.flag == 0) {
                printf(L_BLUE"%s"NONE": %s\n", rmsg.msg.from, rmsg.msg.message);
            } else if (rmsg.msg.flag == 2) {
                printf(YELLOW"通知信息: " NONE "%s\n", rmsg.msg.message);
            } else if (rmsg.msg.flag == 1){
                printf(L_BLUE"%s"L_GREEN"*"NONE": %s\n", rmsg.msg.from, rmsg.msg.message);
            } else {
                printf("Error!\n");
            }
            fflush(stdout);
        }
        wait(NULL);
        close(sockfd);
    }
    return 0;
}

