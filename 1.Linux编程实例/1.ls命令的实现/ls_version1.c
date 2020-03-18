/*************************************************************************
    > File Name: ls.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sun 15 Mar 2020 07:00:42 PM CST
 ************************************************************************/

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>

void do_ls(char[]);

int main(int argc, char *argv[]) {
	if (argc == 1) {
		do_ls(".");
	} else {
		while (--argc) {
			printf("%s:\n", *++argv);
			do_ls(*argv);
		}
	}
	return 0;
}


void do_ls(char dirname[]) {
	//printf("doing ls in %s\n", dirname);
	DIR *dirp;
	struct dirent *direntp;
	if ((dirp = opendir(dirname)) == NULL) {
		perror("opendir");
		return ;
	}
	while ((direntp = readdir(dirp)) != NULL) {
		printf("%s ", direntp->d_name);
	}
	closedir(dirp);
	printf("\n");
}
