#include "../general.h"

int main() {
    int sockfd;
    char name1[MAX_LEN], name2[MAX_LEN];
    struct sockaddr_un serv_addr, client_addr;
    socklen_t serv_len;
    int res_from_serv = 0, cnt_dot_files = 0, new_value = 0;

    printf("[client] cоздаю гнездо...\n");
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

    printf("[client] получаю имя серверного гнезда...\n");
    get_name(name1);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, name1, sizeof(serv_addr.sun_path) - 1);

    get_name2(name2);
    unlink(name2);
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, name2, sizeof(client_addr.sun_path) - 1);
    bind(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr));

    printf("[client] делаю handshake...\n");
    sendto(sockfd, "be", strlen("be") + 1, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    serv_len = sizeof(serv_addr);
    printf("[client] получаю число от сервера...\n");
    recvfrom(sockfd, &res_from_serv, sizeof(res_from_serv), 0, NULL, NULL);

    printf("[client] получаю кол-во файлов, имя которых начинается с точки...\n");
    get_cnt_files(&cnt_dot_files, 1);
    printf("\t%d\n", cnt_dot_files);
    new_value = res_from_serv - cnt_dot_files;

    printf("[client] отправляю инфу серверу...\n");
    sendto(sockfd, &new_value, sizeof(new_value), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    close(sockfd);
    unlink(name2);
    
    return 0;
}