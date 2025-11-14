#include "../headers/general.h"

void create_shm(int *shm_id);
void connect_shm(int shm_id, shm_data_t **adr);
void create_sems(int *sem_id);
void init_sems(int sem_id, unsigned short* val_sems);

void get_info(int sem_id,  shm_data_t *adr);
int more2(char *path);

int main() {
    int shm_id;
    int sem_id;
    unsigned short val_sems[SEM_COUNT] = {1, 0, 0, 0};
    shm_data_t *adr = NULL;

    printf("[server] создаю семафоры и РОП...\n");
    create_shm(&shm_id);
    connect_shm(shm_id, &adr);

    create_sems(&sem_id);
    init_sems(sem_id, val_sems);

    printf("[server] жду клиента...\n");
    get_info(sem_id, adr);

    while (!(adr->done1 && adr->done2)) {
        sleep(1);
    }

    printf("[server] удаляю НС и РОП\n");
    semctl(sem_id, 0, IPC_RMID);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}

void create_shm(int *shm_id) {
    *shm_id = shmget(KEY, sizeof(shm_data_t), IPC_CREAT | 0666);
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
    memset(*adr, 0, sizeof(**adr));

    (*adr)->done1 = (*adr)->done2 = 0;
}


void create_sems(int *sem_id) {
    *sem_id = semget(KEY, SEM_COUNT, IPC_CREAT | 0666);
    if (*sem_id == -1) {
        perror("semget create");
        exit(1);
    }
}

void init_sems(int sem_id, unsigned short* val_sems) {
    union semun sem_union;
    sem_union.array = val_sems;
    if (semctl(sem_id, 0, SETALL, sem_union) == -1) {
        perror("semctl init");
        exit(1);
    }
}

void get_info(int sem_id,  shm_data_t *adr) {
    int flag = 0;

    while (flag < 2) {
        if (sem_down(sem_id, SEM_REQ) == -1) {
            perror("sem_down req");
            exit(1);
        }

        if (sem_down(sem_id, SEM_MUTEX) == -1) {
            perror("sem_down req");
            exit(1);
        }

        int cid = adr->client_id;
        int n_txt = adr->n_names;
        int n_over_2 = 0;

        for (int i = 0; i < n_txt; ++i) {
            char *name = adr->names[i];
            if (more2(name)) {
                strncpy(adr->match_names[n_over_2], name, MAX_NAME - 1);
                n_over_2++;
            }
        }

        adr->n_match = n_over_2;

        if (sem_up(sem_id, SEM_MUTEX) == -1) {
            perror("sem_up mutex");
            exit(1);
        }

        if (cid == 1) {
            if (sem_up(sem_id, SEM_RESP_C1) == -1) {
                perror("sem_up resp_c1");
                exit(1);
            }
        } else if (cid == 2) {
            if (sem_up(sem_id, SEM_RESP_C2) == -1) {
                perror("sem_up resp_c2");
                exit(1);
            }
        }

        flag++;

        printf("[server] обработал запрос от клиента %d\n", cid);
    }
}

int more2(char *path) {
    struct stat file_info;
    if (stat(path, &file_info) == -1) {
        perror("stat");
        exit(1);
    }
    return (file_info.st_blocks > 2);
}