#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <getopt.h>

void do_ls(char []);
void do_lsl(char []);
void do_lsa(char []);
void print_details(char *, char *);
void get_details(char *filename, struct stat *info);
void get_mode(int mode, char str[]);
void get_time(struct stat *);
void get_id(uid_t, gid_t);
void calc_total(char []);//有毛病

int main(int argc, char *argv[]){
    int opt;
	char *optstring = "l::a::";
	opt = getopt(argc, argv, optstring);
	switch(opt) {
		case 'l' : {
			if (argc == 2) {
				do_lsl(".");
			} else {
				argv++;
				while((--argc) >= 2){
					*++argv;
					calc_total(*argv);
					do_lsl(*argv);
				}
			}
			break;
		} 
		case 'a' : {			
			if (argc == 2) {
				do_lsa(".");
			} else {
				argv++;
				while((--argc) >= 2){
					*++argv;
					calc_total(*argv);
					do_lsa(*argv);
				}
			}
			break;
		}
		default : {	
			if (argc == 1) {
			do_ls(".");
			} else {
				while(--argc){
					*++argv;
					calc_total(*argv);
					do_ls(*argv);
				}
			}
			break;
		}
	}
    return 0;
}

void calc_total(char dirname[]) {
	int total = 0;
	DIR *dirp = opendir(dirname);
	struct dirent *direntp;
	struct stat info;
	while ((direntp = readdir(dirp)) != NULL) {
		lstat(dirname, &info);
		int sum = info.st_size / 4096;
		if (info.st_size % 4096 > 0) sum++;
		if (S_ISLNK(info.st_mode)) sum = 0;
		total += sum;
	}
	closedir(dirp);
	printf("total : %d\n", total);
}


void do_ls(char dirname[]){
	printf("doing ls in %s\n", dirname);
    DIR *dirp;
    struct dirent *direntp;
    if ((dirp = opendir(dirname)) == NULL){
		perror("opendir");
	}
    while ((direntp = readdir(dirp)) != NULL){
		if (!strcmp(direntp->d_name, ".")) continue;
		if (!strcmp(direntp->d_name, "..")) continue;
		printf("%s ", direntp->d_name); 
    }
    closedir(dirp);
	printf("\n");
}

void do_lsa(char dirname[]){
	printf("doing ls in %s\n", dirname);
    DIR *dirp;
    struct dirent *direntp;
    if ((dirp = opendir(dirname)) == NULL){
		perror("opendir");
	}
    while ((direntp = readdir(dirp)) != NULL){
        printf("%s ", direntp->d_name);
    }
    closedir(dirp);
	printf("\n");
}

void do_lsl(char dirname[]){
	printf("doing ls in %s\n", dirname);
    DIR *dirp = opendir(dirname);
    struct dirent *direntp;
    if ((dirp = opendir(dirname)) == NULL){
		perror("opendir");
	}
    while ((direntp = readdir(dirp)) != NULL){
		if (!strcmp(direntp->d_name, ".")) continue;
		if (!strcmp(direntp->d_name, "..")) continue;
        print_details(direntp->d_name,dirname);
    }
    closedir(dirp);
	printf("\n");
}

void print_details( char *filename ,char dirname[]){
    struct stat info;
    char tempname[100];
    strcpy(tempname,dirname);
    if(strcmp(dirname,".") == 0){
        strcpy(tempname,filename);
    } else if (strcmp(dirname,"/") == 0){
        strcat(tempname,filename);
    } else {
        strcat(strcat(tempname,"/"), filename);
    }
    if(stat(tempname, &info) != -1){
        get_details(filename, &info);
    } else {
        perror(filename);
    }
}

void get_details(char *filename, struct stat *info){
    char modestr[15];
	get_mode(info->st_mode, modestr);
    printf("%s", modestr);
    printf("%2d ", (int)info->st_nlink); 
    get_id(info->st_uid, info->st_gid);
	printf("%-8ld ", (long) info->st_size);
	get_time(info);
    printf("%-4ld", info->st_blocks/2);
    printf("%s\n", filename);

}
void get_mode(int mode, char str[]){
    if (S_ISDIR(mode)) str[0] = 'd';
	else if (S_ISREG(mode)) str[0] = '-';
	else if (S_ISCHR(mode)) str[0] = 'c';
	else if (S_ISBLK(mode)) str[0] = 'b';
	else if (S_ISFIFO(mode)) str[0] = 'p';
	else if (S_ISLNK(mode)) str[0] = 'l';
	else str[0] = 's';
    if (mode & S_IRUSR) str[1] = 'r'; else str[1] = '-';
    if (mode & S_IWUSR) str[2] = 'w'; else str[2] = '-';
    if (mode & S_IXUSR) str[3] = 'x'; else str[3] = '-';
    if (mode & S_IRGRP) str[4] = 'r'; else str[4] = '-';
    if (mode & S_IWGRP) str[5] = 'w'; else str[5] = '-';
    if (mode & S_IXGRP) str[6] = 'x'; else str[6] = '-';
    if (mode & S_IROTH) str[7] = 'r'; else str[7] = '-';
    if (mode & S_IWOTH) str[8] = 'w'; else str[8] = '-';
    if (mode & S_IXOTH) str[9] = 'x'; else str[9] = '-';

}

void get_id(uid_t uid, gid_t gid){
    struct passwd *user;
	struct group *group;
    char num[10];
	user = getpwuid(uid);
	printf("%s ", user->pw_name);
	group = getgrgid(gid);
	printf("%s ", group->gr_name);
}

void get_time(struct stat *info) {
	struct tm *t = localtime(&info->st_mtime);
	printf("%2d %2d %02d:%02d ", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);

}


