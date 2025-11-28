#include <sys/un.h> // struct sockaddr_un
#include <unistd.h> // getcwd
#include <string.h> // memset strncpy

#include <dirent.h> // DIR, struct dirent, opendir, readdir, closedir
#include <sys/stat.h> // struct stat, stat, макрос S_ISREG

#include <stdio.h> // printf
#include <sys/socket.h> // socket/bind/recvfrom/sendto

#include "general.h"

void set_name(char name[MAX_LEN], struct sockaddr_un *serv_addr);
void get_cnt_files(int *cnt);

int main() {
    char name[MAX_LEN];
    struct sockaddr_un serv_addr; // содержит информацию о локальном адресе для привязки
    set_name(name, &serv_addr);


    int sockfd;
    printf("[server] cоздаю гнездо...\n");
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);


    printf("[server] присваиваю ему имя...\n");
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));


    struct sockaddr_un client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_len = sizeof(client_addr);
    char buf[3];
    printf("[server] жду первое сообщение от клиента (handshake)...\n");
    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_len);

    int cnt_files = 0;
    printf("[server] считаю кол-во файлов текущего каталога...\n");
    get_cnt_files(&cnt_files);
    printf("\t%d\n", cnt_files);


    printf("[server] отправляю инфу клиенту...\n");
    sendto(sockfd, &cnt_files, sizeof(cnt_files), 0, (struct sockaddr *)&client_addr, client_len);


    int res_from_client;
    printf("[server] получаю инфу от клиента...\n");
    recvfrom(sockfd, &res_from_client, sizeof(res_from_client), 0, NULL, NULL);
    printf("\t%d\n", res_from_client);


    printf("[server] завершение работы...\n");
    close(sockfd);
    unlink(name);

    return 0;
}

void set_name(char name[MAX_LEN], struct sockaddr_un *serv_addr) {
    get_name(name);
    unlink(name);

    memset(serv_addr, 0, sizeof(*serv_addr));
    serv_addr->sun_family = AF_UNIX;
    strncpy(serv_addr->sun_path, name, sizeof(serv_addr->sun_path)-1);
}

void get_cnt_files(int *cnt) {
    DIR *dir; //  представляет поток каталога
    struct dirent *entry; // структура данных для элементов внутри директории.

    dir = opendir("."); // возвращает указатель на открытый поток директории

    while ((entry = readdir(dir)) != NULL) { // readdir читает следующую запись из каталога и возвращает указатель на структуру, содержащую информацию о файле.
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // пропускаем служебные записи . и ..
            continue;

        (*cnt)++;
    }

    closedir(dir);
}
