#ifndef GENERAL_H
#define GENERAL_H

#include <sys/socket.h> // socket AF_INET SOCK_STREAM sockaddr bind
#include <stdio.h>      // perror
#include <stdlib.h>     // exit
#include <string.h> // memset strncpy
#include <unistd.h> // close unlink getcwd
#include <dirent.h> // DIR, struct dirent, opendir, readdir, closedir


#define MAX_LEN 2048

void get_name(char name[MAX_LEN]) {
    char cwd[MAX_LEN];
    getcwd(cwd, MAX_LEN);
    snprintf(name, MAX_LEN, "%s/%s", cwd, "serv.sock");
}

void get_name2(char name[MAX_LEN]) {
    char cwd[MAX_LEN];
    getcwd(cwd, MAX_LEN);
    snprintf(name, MAX_LEN, "%s/%s", cwd, "client.sock");
}

void get_cnt_files(int *cnt, int flag) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");

    while ((entry = readdir(dir)) != NULL) { // readdir читает следующую запись из каталога и возвращает указатель на структуру, содержащую информацию о файле.
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        if (flag == 0) {
            (*cnt)++;
        } else if (entry->d_name[0] == '.') {
            (*cnt)++;
        }
    }

    closedir(dir);
}

#endif