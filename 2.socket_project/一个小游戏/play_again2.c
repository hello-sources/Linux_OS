/*************************************************************************
    > File Name: play_again1.c
    > Author: ltw
    > Mail: 3245849061@qq.com 
    > Created Time: Thu 28 May 2020 08:00:20 PM CST
 ************************************************************************/

#include <stdio.h>
#include <termios.h>
#define Q "Do you want another try?"

int get_response();
void tty_mode(int);
void set_cr_mode();

int main() {
	int response;
	tty_mode(0);
	set_cr_mode();
	response = get_response();
	tty_mode(1);
	printf("\nreturn val = %d\n", response);
	return response;
}

int get_response() {
	printf("%s", Q);
	printf("(y/n)");
	switch (getchar()) {
		case 'y' :
		case 'Y' : return 1;
		case 'N' :
		case 'n' : return 0;
		default : return 0; 
	}
	
}

void set_cr_mode() {
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	ttystate.c_cc[VTIME] = 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how) {
	//how = 1;还原 how = 0;保存
	static struct termios original_mode;
	if (how == 0) {
		tcgetattr(0, &original_mode);
	} else {
		tcsetattr(0, TCSANOW, &original_mode);
	}
}
