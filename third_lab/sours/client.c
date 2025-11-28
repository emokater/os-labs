#include "general.h"

#include <dirent.h> // DIR, struct dirent, opendir, readdir, closedir
#include <sys/stat.h> // struct stat, stat, макрос S_ISREG
#include <sys/un.h> // struct sockaddr_un
#include <sys/types.h>
#include <unistd.h>

void get_cnt_files(int *cnt);

int main() {
    char name[MAX_LEN];
    struct sockaddr_un serv_addr;
    printf("[client] получаю имя серверного гнезда...\n");
    get_name(name);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, name, sizeof(serv_addr.sun_path) - 1);

    printf("[client] cоздаю гнездо...\n");
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

    char client_name[MAX_LEN];
    struct sockaddr_un client_addr;
    snprintf(client_name, MAX_LEN, "/tmp/client-%d.sock", getpid());
    unlink(client_name);
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, client_name, sizeof(client_addr.sun_path) - 1);

    if (bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
        perror("bind client");
        close(sockfd);
        return 1;
    }

    printf("[client] делаю handshake...\n");
    sendto(sockfd, "be", strlen("be") + 1, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));


    int res_from_serv = 0;
    socklen_t serv_len = sizeof(serv_addr);
    printf("[client] получаю число от сервера...\n");
    recvfrom(sockfd, &res_from_serv, sizeof(res_from_serv), 0, NULL, NULL);
    printf("\tпринято: %d\n", res_from_serv);

    int cnt_dot_files = 0, new_value = 0;
    printf("[client] получаю кол-во файлов, имя которых начинается с точки...\n");
    get_cnt_files(&cnt_dot_files);
    printf("\t%d\n", cnt_dot_files);
    new_value = res_from_serv - cnt_dot_files;

    printf("[client] отправляю инфу серверу...\n");
    sendto(sockfd, &new_value, sizeof(new_value), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("[client] завершение работы...\n");
    close(sockfd);
    unlink(client_name);
    
    return 0;
}

void get_cnt_files(int *cnt) {
    DIR *dir; //  представляет поток каталога
    struct dirent *entry; // структура данных для элементов внутри директории.

    dir = opendir("."); // возвращает указатель на открытый поток директории

    while ((entry = readdir(dir)) != NULL) { // readdir читает следующую запись из каталога и возвращает указатель на структуру, содержащую информацию о файле.
        // пропускаем служебные записи . и ..
        // if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        //     continue;
        
        if (entry->d_name[0] == '.') {
            (*cnt)++;
        }
    }

    closedir(dir);
}