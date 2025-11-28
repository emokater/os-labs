#ifndef GENERAL_H
#define GENERAL_H

#include <sys/socket.h> // socket AF_INET SOCK_STREAM sockaddr bind
#include <stdio.h>      // perror
#include <stdlib.h>     // exit
#include <string.h> // memset strncpy
#include <unistd.h> // close unlink getcwd

#define MAX_LEN 2048

void get_name(char name[MAX_LEN]) {
    char cwd[MAX_LEN];
    getcwd(cwd, MAX_LEN);
    snprintf(name, MAX_LEN, "%s/%s", cwd, "socket_file");
}

#endif