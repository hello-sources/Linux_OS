/*************************************************************************
    > File Name: play_again1.c
    > Author: ltw
    > Mail: 3245849061@qq.com 
    > Created Time: Thu 28 May 2020 08:00:20 PM CST
 ************************************************************************/

#include <stdio.h>
#define Q "Do you want another try?"

int getresponse();

int main() {
	int response;
	response = getresponse();

	return getresponse();
}

int getresponse() {
	printf("%s", Q);
	printf("(y/n) : ");
	switch (getchar()) {
		case 'y' :
		case 'Y' : return 1;
		case 'N' :
		case 'n' : return 0;
		default : return 0; 
	}
	
}
