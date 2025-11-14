#ifndef CLIENTS_H
#define CLIENTS_H

#include "general.h"

void get_shm(int *shm_id) {
    *shm_id = shmget(KEY, sizeof(shm_data_t), 0666);
    if (*shm_id == -1) {
        perror("shmget error");
        exit(1);
    }
}

void connect_shm(int shm_id, shm_data_t **adr) {
    *adr = (shm_data_t *)shmat(shm_id, NULL, 0);
    if (*adr == (void *)-1) { 
        perror("shmat");
        exit(1); 
    }
}

void get_sems(int *sem_id) {
    *sem_id = semget(KEY, SEM_COUNT, 0666);
    if (*sem_id == -1) {
        perror("semget");
        exit(1);
    }
}

void get_files(char files[MAX_FILES][MAX_NAME], int *count, const char *ext) {
    char cmd[64];

    snprintf(cmd, sizeof(cmd), "ls *.%s 2>/dev/null", ext);

    FILE *pf = popen(cmd, "r");
    if (!pf) {
        perror("popen");
        exit(1);
    }

    while (fgets(files[*count], MAX_NAME, pf)) {
        size_t len = strlen(files[*count]);

        if (len > 0 && files[*count][len - 1] == '\n')
            files[*count][len - 1] = '\0';

        (*count)++;
    }

    pclose(pf);

    for (int i = 0; i < (*count); ++i) {
        printf("\t%s\n", files[i]);
    }
}

void write_in_shm(int sem_id, shm_data_t *adr, char files[MAX_FILES][MAX_NAME], int cnt_files, int client_id) {
    if (sem_down(sem_id, SEM_MUTEX) == -1) {
        perror("sem_down mutex");
        exit(1);
    }

    adr->client_id = client_id;
    adr->n_names = cnt_files;

    for (int i = 0; i < cnt_files; ++i) {
        strncpy(adr->names[i], files[i], MAX_NAME - 1);
    }

    if (sem_up(sem_id, SEM_MUTEX) == -1) {
        perror("sem_up mutex");
        exit(1);
    }

    if (sem_up(sem_id, SEM_REQ) == -1) {
        perror("sem_up req");
        exit(1);
    }
}

void get_results_from_shm(int sem_id, shm_data_t *adr, enum sems n_client, int n_files, int cid) {
    if (sem_down(sem_id, n_client) == -1) {
        perror("sem_down resp");
        exit(1);
    }

    if (sem_down(sem_id, SEM_MUTEX) == -1) {
        perror("sem_down mutex");
        exit(1);
    }

    printf("[client %d] количество файлов, подходящих по условию: %d из %d\n", cid, adr->n_match, n_files);
    for (int i = 0; i < adr->n_match; ++i) {
        printf("\t%s\n", adr->match_names[i]);
    }

    if (n_client == SEM_RESP_C1) {
        adr->done1 = 1;
    } else {
        adr->done2 = 1;
    }

    if (sem_up(sem_id, SEM_MUTEX) == -1) {
        perror("sem_up mutex");
        exit(1);
    }

}

time_t disconnect_shm(shm_data_t *adr) {
    int res = shmdt(adr);
    time_t t = time(NULL);

    if (res == -1) {
        perror("shmdt");
        exit(1);
    }

    return t;
}

#endif