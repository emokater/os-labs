#include "../general.h"

int main() {
    int sockfd;
    char name[MAX_LEN];
    struct sockaddr_un serv_addr, client_addr;
    socklen_t client_len;
    char buf[3];
    int cnt_files = 0, res_from_client = 0;

    printf("[server] cоздаю гнездо...\n");
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

    printf("[server] присваиваю ему имя...\n");
    get_name(name);
    unlink(name);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, name, sizeof(serv_addr.sun_path)-1);
    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("[server] жду первое сообщение от клиента (handshake)...\n");
    memset(&client_addr, 0, sizeof(client_addr));
    client_len = sizeof(client_addr);
    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &client_len);

    printf("[server] считаю кол-во файлов текущего каталога...\n");
    get_cnt_files(&cnt_files, 0);
    printf("\t%d\n", cnt_files);

    printf("[server] отправляю инфу клиенту...\n");
    sendto(sockfd, &cnt_files, sizeof(cnt_files), 0, (struct sockaddr *)&client_addr, client_len);

    printf("[server] получаю инфу от клиента...\n");
    recvfrom(sockfd, &res_from_client, sizeof(res_from_client), 0, NULL, NULL);
    printf("[server] результат: %d\n", res_from_client);

    close(sockfd);
    unlink(name);

    return 0;
}
