#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

#define MAX_CNT_FILES 1024
#define MSGMAX 2048
#define MSG_KEY 4

struct file_info {
    char name[PATH_MAX];
    time_t ctime;
};

struct msgbuf {
    long mtype;
    char text[MSGMAX];
};

void openQueue(int *qid);
void getMsgs(int qid, char folderName[PATH_MAX], struct file_info *files, int max_files, int *file_count);
int comparator(const void *a, const void *b);
time_t get_creation_time(const char *path);
void filesProcessing(struct file_info *files, int file_count);
void getQueueInfo(int qid);
void delQueue(int qid);

int main() {
    printf("\nСервер:\n");

    printf("1. Открываю очередь сообщений...\n");
    int qid;
    sleep(1);
    openQueue(&qid);

    printf("2. Выбираю из очереди все сообщения...\n");
    char folderName[PATH_MAX];
    struct file_info files[MAX_CNT_FILES];
    int file_count = 0;
    getMsgs(qid, folderName, files, MAX_CNT_FILES, &file_count);

    printf("3. Вывожу информацию о текущем имени каталога...\n\n");
    if (folderName[0] == '\0') { 
        printf("\tне получено имя текущего каталога!\n\n"); 
    } else {
        printf("\t%s\n\n", folderName);
    }

    printf("4. Вывожу отсортированную информацию о времени создания файлов...\n\n");
    if (file_count == 0) {
        printf("\tне получено ни одного имени файла!\n\n");    
    } else {
        filesProcessing(files, file_count);
    }

    printf("5. Посмотрим информацию об очереди...\n\n");
    getQueueInfo(qid);

    printf("\n6. Удаляю очередь сообщений...");
    delQueue(qid);

    return 0;
}

void openQueue(int *qid) {
    *qid = msgget(MSG_KEY, 0); 
    if (*qid == -1) {
        perror("Ошибка открытия очереди сообщений :(");
        exit(1);
    }
}

time_t get_creation_time(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        perror("stat");
        return 0;
    }

#if defined(__APPLE__) && defined(st_birthtime)
    return st.st_birthtime; 
#elif defined(__linux__)
    return st.st_ctime;
#endif
}

void getMsgs(int qid, char folderName[PATH_MAX], struct file_info *files, int max_files, int *file_count) {
    struct msgbuf msg;
    memset(folderName, 0, PATH_MAX);

    int res = msgrcv(qid, &msg, sizeof(msg.text), 0, IPC_NOWAIT);

    while (res != -1) {
        if (msg.mtype == 1) {
            strncpy(folderName, msg.text, strlen(msg.text)+1);
        } else if (msg.mtype == 2 && *file_count < max_files) {
            strncpy(files[*file_count].name, msg.text, strlen(msg.text)+1);
            files[*file_count].ctime = get_creation_time(msg.text);
            (*file_count)++;
        }
        res = msgrcv(qid, &msg, sizeof(msg.text), 0, IPC_NOWAIT);
    }
}

int comparator(const void *a, const void *b) {
    int res = 0;
    const struct file_info *fa = a;
    const struct file_info *fb = b;
    if (fa->ctime < fb->ctime) { res = -1; }
    if (fa->ctime > fb->ctime) { res = 1; }
    return res;
}

void filesProcessing(struct file_info *files, int file_count) {
    qsort(files, file_count, sizeof(struct file_info), comparator);
    for (int i=0; i < file_count; i++) {
        printf("\tфайл: %s, время создания: %s\n", files[i].name, ctime(&files[i].ctime));
    }
}

void getQueueInfo(int qid) {
    struct msqid_ds info;
    if (msgctl(qid, IPC_STAT, &info) == -1) {
        perror("msgctl IPC_STAT");
    } else {
        printf("\tPID последнего процесса, отправившего сообщение: %d\n", info.msg_lspid);
        printf("\tМаксимальная длина очереди (байт): %zu\n", (size_t)info.msg_qbytes);
    }
}

void delQueue(int qid) {
    if (msgctl(qid, IPC_RMID, NULL) == -1) {
        perror("msgctl IPC_RMID");
    } else {
        printf(" очередь сообщений успешно удалена.\n\n");
    }
}