#include <sys/ipc.h> // IPC_CREAT
#include <sys/msg.h> // msgget
#include <stdlib.h> // exit
#include <stdio.h> // perror
#include <string.h> // memset
#include <sys/types.h> // key_t
#include <unistd.h> // getcwd
#include <limits.h> // PATH_MAX

#define MSGMAX 2048
#define MSG_KEY 4

struct msgbuf {
    long mtype;
    char text[MSGMAX];
};

int createMQ(key_t key);
void getTmpFolderName(char data[MSGMAX]);
void sendMsg(long mtype_, char *text_, int fd, int flag);

int main() {
    printf("Клиент:\n");
    int fd, n;
    char nameCurFolder[PATH_MAX], line[MSGMAX];
    memset(nameCurFolder, 0, PATH_MAX);

    printf("1. Cоздаю очередь сообщений...\n");
    fd = createMQ(MSG_KEY);

    printf("2. Получаю полное имя текущего каталога и передаю его в очередь...\n");
    getTmpFolderName(nameCurFolder);
    sendMsg(1, nameCurFolder, fd, 0);

    printf("3. Получаю список файлов текущего каталога в которых встречается подстрока «define» и передаю их в очередь...\n");
    FILE *pf = popen("grep -rl --exclude-dir=tests --exclude-dir=output --exclude-dir=.git 'define' .", "r");
    if (!pf) { perror("Ошибка popen :("); exit(1); }

    while (fgets(line, MSGMAX, pf)) {
        size_t len = strlen(line);
        if (len && line[len-1] == '\n') { line[len-1] = '\0'; }
        if (line[0] == '.' && line[1] == '/') { memmove(line, line+2, strlen(line+2) + 1);}
        if (line[0] == '\0') { continue; }
        sendMsg(2, line, fd, 0);
    }

    if (pclose(pf) == -1) {
        perror("Ошибка закрытия потока :(");
        exit(1);
    }

    return 0;
}

int createMQ(key_t key) {
    int fd = msgget(key, IPC_CREAT | 0666);
    if (fd == -1) { 
        perror("Ошибка создания очереди :("); 
        exit(1); 
    }
    return fd;
}

void getTmpFolderName(char data[MSGMAX]) {
    char *res = getcwd(data, MSGMAX);
    if (!res) {
        perror("Ошибка получения имени текущего каталога :(");
        exit(1);
    }
}

void sendMsg(long mtype_, char *text_, int fd, int flag) {
    struct msgbuf msg;
    memset(msg.text, 0, MSGMAX);

    msg.mtype = mtype_;

    size_t len = strnlen(text_, MSGMAX - 1);
    memcpy(msg.text, text_, len);
    msg.text[len] = '\0';

    int res = msgsnd(fd, &msg, strlen(text_)+1, flag);
    if (res == -1) {
        perror("Ошибка передачи сообщения в очередь :(");
        exit(1);
    }
}