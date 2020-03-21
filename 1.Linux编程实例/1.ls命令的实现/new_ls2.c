/*************************************************************************
    > File Name: new_ls1.c
    > Author: ma6174
    > Mail: ma6174@163.com 
    > Created Time: Sat 21 Mar 2020 03:53:26 PM CST
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

#define FILEMAX 1024
#define NAMEMAX 256

int flag_a = 0;//标记是否传入参数a
int flag_l = 0;//标记是否传入参数l
int dir_num = 0;
int fg_c, bg_c;//前景色与背景色,设置为全局变量便于不同文件类型更改

//统计当前终端的窗口大小，分别为行，列
void size_window(char filename[][NAMEMAX], int cnt, int* row, int* col){
    struct winsize size;//winsize结构体有窗口长宽定义
    int len[cnt], max = 0, total = 0;
    memset(len, 0, sizeof(int) * cnt);
    if (isatty(STDOUT_FILENO) == 0) {
        exit(1);
    }

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
        perror("ioctl");
        exit(1);
    }

    for (int i= 0; i < cnt; i++){
        len[i] = strlen(filename[i]);
        if (max < len[i]) max = len[i];
        total += len[i] + 1;
    }
    if (max + 1 >= size.ws_col) {
        *row = cnt;
        *col = 1;
        return ;
    }
    if (total <= size.ws_col) {
        *row = 1;
        *col = cnt;
        return ;
    }

    int try_begin = 0;
    for (int i = 0, tmp = 0; i < cnt; i++) {
        tmp += (len[i] + 1);
        if (tmp >= size.ws_col) {
            try_begin = i;
            break;
        }
    }

    for (int i = try_begin; ;i--) {
        int *wide = (int *)malloc(sizeof(int) * i);
        memset(wide, 0, sizeof(int) * i);
        *row = (int)ceil(cnt / i);
        int try_sum = 0;
        for (int x = 0; x < i; x++) {
            for (int y = x * *row; y < (x + 1) * *row && y < cnt; y++) {
                if (wide[x] < len[y]) wide[x] = len[y];
            }
            try_sum += (wide[x] + 1);
        }

        if (try_sum > size.ws_col) continue;
        if (try_sum <= size.ws_col) {
            *col = i;
            break;
        }
    }
}

//根据文件名称大小，来判断当前列的大小应该为多少
void show_files(char filename[][NAMEMAX], int cnt, int row, int col) {
    int wide_file[cnt];
    struct stat tmp_st;
	memset(wide_file, 0, sizeof(int) * cnt);
    for(int i = 0; i < col; i++) {
        for (int j = (i * row); j < (i + 1) * row && j < cnt; j++) {
            if (wide_file[i] < strlen(filename[j])) wide_file[i] = strlen(filename[j]) ;

        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = i; j < i + (row * col) && j < cnt; j = j + row) {
            int tmp = j / row;
			lstat(filename[j], &tmp_st);
			update_color(tmp_st.st_mode);
            printf("\033[%d;%dm%-*s\033[0m", bg_c, fg_c, wide_file[tmp] + 1, filename[j]);
        }
            printf("\n");
        }
}

//更新C语言printf对应的颜色数，用来表示不同的文件类型
void update_color(mode_t mode) {
	bg_c = 0;
	fg_c = 37;
	if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
		fg_c = 32;
	}
	switch (mode & S_IFMT) {
		case S_IFDIR :
			fg_c = 34;
			break;
		case S_IFLNK :
			fg_c = 36;
			break;
	}
}

//输出的是对应ls -l选项下的包括文件类型以及文件权限
void mode_to_str(mode_t mode, char *str) {
    if (S_ISREG(mode)) str[0] = '-';
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';
    if (S_ISSOCK(mode)) str[0] = 's';
    if (S_ISLNK(mode)) str[0] = 'l';
    if (S_ISFIFO(mode)) str[0] = 'p';

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';

    if ((mode & S_IXUSR) && (mode & S_ISUID)) str[3] = 's';
	update_color(mode);
}

//获取当前文件的uid
char *uid_to_name(uid_t uid){
    struct passwd *pw_ptr;
    static char tmpstr[10] = {0};
    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(tmpstr, "%d", uid);
        return tmpstr;
    } else {
        return pw_ptr->pw_name;
    }
}

//获取当前文件的gid
char *gid_to_name(gid_t gid) {
    struct group *gr_ptr;
    static char tmpstr[10] = {0};
    if ((gr_ptr = getgrgid(gid)) == NULL) {
        sprintf(tmpstr, "%d", gid);
        return tmpstr;
    } else {
        return gr_ptr->gr_name;
    }
}

//输出ls -l中的详细信息
void show_info(char *filename, struct stat *info) {
    char modestr[11] = "----------";
    mode_to_str(info->st_mode, modestr);
    printf("%s ", modestr);
    printf("%4d ", info->st_nlink);
    printf("%8s ", uid_to_name(info->st_uid));
    printf("%8s ", gid_to_name(info->st_gid));
    printf("%8lld ", info->st_size);
    printf("%.15s ",  4 + ctime(&info->st_mtime));
    printf("\033[%d;%dm%s\033[0m ", bg_c, fg_c, filename);
	//处理软连接输出问题
	if (modestr[0] == 'l') {
		int cnt;
		char buf[NAMEMAX] = {0};
		if (cnt = readlink(filename, buf, NAMEMAX) < 0) {
			perror("readlink");
		} else {
			printf("-> \033[%d;%dm%s\033[0m", bg_c, fg_c, buf);
		}
	}
	printf("\n");
}

//当前文件的所属属性
void do_stat(char *filename) {
    struct stat st;
    if (lstat(filename, &st) < 0) {
        perror(filename);
    } else {
        show_info(filename, &st);
    }
}

//比较名称，用来文件排序
int cmp_name(const void* _a, const void* _b) {
    char *a = (char *)_a;
    char *b = (char *)_b;
    return strcmp(a, b);
}

//命令选项
void do_ls(char *dirname) {
    DIR *dirp = NULL;
    struct dirent *direntp;
    char names[FILEMAX][NAMEMAX] = {0};
    if ((dirp = opendir(dirname)) == NULL) {
        if (access(dirname, R_OK) == 0) {
            if (flag_l == 0) {
                dir_num--;
                struct stat tmp_st;
				update_color(tmp_st.st_mode);
				printf("\033[%d;%dm%s\033[0m\n", bg_c, fg_c, dirname);
                return;
            } else {
                do_stat(dirname);
                return;
            }
        } else {
            perror(dirname);
            return ;
        }
    } else {
        if (dir_num)
            printf("%s:\n", dirname);
        chdir(dirname);
        int cnt = 0;
        while((direntp = readdir(dirp)) != NULL) {
            if (direntp->d_name[0] == '.' && (flag_a == 0)) continue;
            strcpy(names[cnt++], direntp->d_name);
        }
        qsort(names, cnt, NAMEMAX, cmp_name);

        if (flag_l == 1) {
            for (int i = 0; i < cnt; i++) {
                do_stat(names[i]);
            }
        } else {
            int row, col;
            size_window(names, cnt, &row, &col);
            show_files(names, cnt, row, col);
        }

    }

}

//主函数
int main(int argc, char **argv) {
    int opt;
    while((opt = getopt(argc, argv, "al")) != -1) {
        switch (opt) {
            case 'a':
            flag_a = 1;
                break;
            case 'l':
                flag_l = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-al] [filename]\n", argv[0]);
                exit(1);
        }
    }
    argc -= (optind - 1);
    argv += (optind - 1);

    //printf("argc = %d \nargv=%s\n",argc,  *(argv + 1));
    if (argc == 1) {
        dir_num = 0;
        do_ls(".");
    } else {
        dir_num = argc - 1;
        while(--argc) {
            do_ls(*(++argv));
        }
    }

    return 0;
}
