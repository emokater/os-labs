#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>

#define MSGMAX 2048
#define MSG_KEY 4

struct msgbuf {
    long mtype;
    char text[MSGMAX];
};

int createMQ(key_t key);
void getTmpFolderName(char data[MSGMAX]);
void sendMsg(long mtype_, char *text_, int qid, int flag);

int main() {
    printf("Клиент:\n");
    int qid;
    char nameCurFolder[PATH_MAX], line[MSGMAX];
    memset(nameCurFolder, 0, PATH_MAX);

    printf("1. Cоздаю очередь сообщений...\n");
    qid = createMQ(MSG_KEY);

    printf("2. Получаю полное имя текущего каталога и передаю его в очередь...\n");
    getTmpFolderName(nameCurFolder);
    sendMsg(1, nameCurFolder, qid, 0);

    printf("3. Получаю список файлов текущего каталога в которых встречается подстрока «define» и передаю их в очередь...\n");
    FILE *pf = popen("grep -rl --exclude-dir=tests --exclude-dir=output --exclude-dir=.git 'define' .", "r");
    if (!pf) { 
        perror("Ошибка popen :("); 
        exit(1); 
    }

    while (fgets(line, MSGMAX, pf)) {
        size_t len = strlen(line);
        if (len && line[len-1] == '\n') { line[len-1] = '\0'; }
        if (line[0] == '.' && line[1] == '/') { memmove(line, line+2, strlen(line+2) + 1);}
        if (line[0] == '\0') { continue; }
        sendMsg(2, line, qid, 0);
    }

    if (pclose(pf) == -1) {
        perror("Ошибка закрытия потока :(");
        exit(1);
    }

    return 0;
}

int createMQ(key_t key) {
    int qid = msgget(key, IPC_CREAT | 0666);
    if (qid == -1) { 
        perror("Ошибка создания очереди :("); 
        exit(1); 
    }
    return qid;
}

void getTmpFolderName(char data[MSGMAX]) {
    char *res = getcwd(data, MSGMAX);
    if (!res) {
        perror("Ошибка получения имени текущего каталога :(");
        exit(1);
    }
}

void sendMsg(long mtype_, char *text_, int qid, int flag) {
    struct msgbuf msg;
    memset(msg.text, 0, MSGMAX);

    msg.mtype = mtype_;

    size_t len = strnlen(text_, MSGMAX - 1);
    memcpy(msg.text, text_, len);
    msg.text[len] = '\0';

    int res = msgsnd(qid, &msg, strlen(text_)+1, flag);
    if (res == -1) {
        perror("Ошибка передачи сообщения в очередь :(");
        exit(1);
    }
}