/*************************************************************************
    > File Name: more2.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 21 Mar 2020 07:30:00 PM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#define PAGELINE 10
#define LENLINE 512

int col, row;

void do_more(FILE *fp);
void get_size();

int main(int argc, char **argv) {
	FILE *fp;
	if (argc == 1) {
		printf("Usage : %s [filename | stdin]\n", argv[0]);
		do_more(stdin);
	} else {
		while (--argc) {
			if ((fp = fopen(*++argv, "r")) != NULL) {
				do_more(fp);
			}
		}
	}

	return 0;
}


void do_more(FILE *fp) {
	FILE *cmd = fopen("/dev/tty", "r");
	int num_line = 0, reply, get_cmd(FILE*);
	get_size();
	char line[LENLINE] = {0};
	while (fgets(line, LENLINE, fp)) {
		if (num_line == row) {
			reply = get_cmd(cmd);
			if (reply == 0) break;
			num_line -= reply;
		}
		if (fputs(line, stdout) == EOF) {
			perror("fputs");
			exit(1);
		}
		num_line++;
	}
}

int get_cmd(FILE *fp) {
	printf("more : ");
	int c;
	while ((c = fgetc(fp)) != EOF) {
		if (c == 'q') return 0;
		if (c == ' ') return col;
		if (c == '\n') return 1;
	}
	return -1;
}


void get_size() {
	struct winsize size;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
	col = size.ws_col;
	row = size.ws_row;
}
