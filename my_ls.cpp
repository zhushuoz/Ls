/*************************************************************************
	> File Name: test_ls.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月21日 星期五 20时48分52秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <math.h>

int gid_len = 0, uid_len = 0, size_len = 0, nl_len = 0;
char str[50];

void len (DIR *dir) {
    struct dirent *rent;
    FILE *fout = fopen("/dev/null", "w");
    while (rent = readdir(dir)) {
        int l;
        struct stat e;
        stat(rent->d_name, &e);
        struct group *grp_p = getgrgid(e.st_gid);
        if (gid_len < (l = strlen(grp_p->gr_name))) gid_len = l;
        struct passwd *pw_ptr = getpwuid(e.st_uid);
        if (uid_len < (l = strlen(pw_ptr->pw_name))) uid_len = l;
        if (size_len < (l = log10(e.st_size) + 1)) size_len = l;
        if (nl_len < (l = log10(e.st_nlink) + 1)) nl_len = l;
       
    }

}
char *find_qx(int mode) {//求文件权限
    char *str;
    str = (char *)malloc(sizeof(char) * 10);
    if (S_ISDIR(mode) == 1) str[0] = 'd'; else str[0] = '-';
    if (mode & S_IRUSR)  str[1] = 'r'; else str[1] = '-';
    if (mode & S_IWUSR)  str[2] = 'w'; else str[2] = '-';
    if (mode & S_IXUSR)  str[3] = 'x'; else str[3] = '-';
    if (mode & S_IRGRP)  str[4] = 'r'; else str[4] = '-';
    if (mode & S_IWGRP)  str[5] = 'w'; else str[5] = '-';
    if (mode & S_IXUSR)  str[6] = 'x'; else str[6] = '-';
    if (mode & S_IROTH)  str[7] = 'r'; else str[7] = '-';
    if (mode & S_IWOTH)  str[8] = 'w'; else str[8] = '-';
    if (mode & S_IXOTH)  str[9] = 'x'; else str[9] = '-';
    for (int i = 0; i < 10; i++) {
        printf("%c", str[i]);
    }
    printf(" ");
    return str;
}


void uid_name(uid_t uid) {//求文件所有者
    struct passwd *pw_ptr;
    pw_ptr = getpwuid(uid);
    printf("%-*s", uid_len + 1, pw_ptr->pw_name);
}


void gid_name(gid_t gid) {//寻找文件所有组
    struct group *grp_p;
    grp_p = getgrgid(gid);
    printf("%-*s", gid_len + 1,grp_p->gr_name);
}

void output(struct dirent *rent, struct stat e) {
    char *str;
    str = (char *)malloc(sizeof(char) * 11);
    str = find_qx(e.st_mode); //输出文件类型和权限
    printf("%-*d", nl_len + 1, (int)e.st_nlink);
    uid_name(e.st_uid); //输出文件所有者
    gid_name(e.st_gid); //输出文件的所有组 
    printf("%*d ", size_len, (int)e.st_size);//输出文件大小
    printf("%.12s",ctime(&e.st_mtime) + 4);
    if (str[0] == 'd')  printf("\e[1;34m %s\n\e[0m", rent->d_name);
    else if (str[3] == 'x' || str[6] == 'x' || str[9] == 'x')       printf("\e[1;32m %s\n\e[0m", rent->d_name);
    else printf(" %s\n", rent->d_name);

}

void ls_al(DIR *dir) {
    struct dirent *rent;
    while (rent = readdir(dir)) {
        struct stat e;
        stat(rent->d_name, &e);
        output(rent, e); 
    }
}

int main() {
    DIR *dir;
    dir = opendir(".");
    len(dir);
    dir = opendir(".");

    ls_al(dir);
    return 0;
}
