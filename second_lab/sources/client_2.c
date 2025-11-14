#include "../headers/clients.h"

int main() {
    int shm_id;
    int sem_id;
    shm_data_t *adr = NULL;
    char txt_files[MAX_FILES][MAX_NAME];
    int cnt_c = 0;

    printf("[client 2] подключаюсь к семафорам и РОП...\n");
    get_shm(&shm_id);
    connect_shm(shm_id, &adr);
    get_sems(&sem_id);

    printf("[client 2] получаю имена Си файлов текущего каталога...\n");
    get_files(txt_files, &cnt_c, "c");

    printf("[client 2] записываю их в РОП...\n");
    write_in_shm(sem_id, adr, txt_files, cnt_c, 2);
    
    printf("[client 2] получаю результаты обработки сервера...\n");
    get_results_from_shm(sem_id, adr, SEM_RESP_C2, cnt_c, 2);

    printf("[client 2] отсоединяюсь от РОП...\n");
    disconnect_shm(adr);

    return 0;
}