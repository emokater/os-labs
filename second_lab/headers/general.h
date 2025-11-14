#ifndef GENERAL_H
#define GENERAL_H

#include <sys/shm.h> // shmget shmat
#include <sys/ipc.h> // IPC_CREAT
#include <sys/sem.h> // shmget() SETALL semid_ds struct sembuf semop() semun
#include <stdio.h> // NULL
#include <stdlib.h> // exit
#include <string.h> // memset
#include <time.h> // time_t time ctime
#include <sys/stat.h> // struct stat
#include <unistd.h> // sleep

#define SEM_COUNT 4
#define KEY 7
#define MAX_FILES 1024
#define MAX_NAME 255

enum sems { SEM_MUTEX = 0, SEM_REQ = 1, SEM_RESP_C1 = 2, SEM_RESP_C2 = 3 };

#if defined(__linux__)
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
#endif

typedef struct {
    int client_id;                    
    int n_names;                   
    char names[MAX_FILES][MAX_NAME];   

    int n_match;          
    char match_names[MAX_FILES][MAX_NAME]; 

    int done1;
    int done2;
} shm_data_t;

int sem_down(int semid, int n) { 
    struct sembuf op = {n, -1, 0};
    return semop(semid, &op, 1);
}

int sem_up(int semid, int n) {
    struct sembuf op = {n, +1, 0};
    return semop(semid, &op, 1);
}

#endif