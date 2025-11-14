#include "../headers/clients.h"

int main() {
    int shm_id;
    int sem_id;
    shm_data_t *adr = NULL;
    char txt_files[MAX_FILES][MAX_NAME];
    int cnt_txt = 0;

    printf("[client 1] подключаюсь к семафорам и РОП...\n");
    get_shm(&shm_id);
    connect_shm(shm_id, &adr);
    get_sems(&sem_id);

    printf("[client 1] получаю имена текстовых файлов текущего каталога...\n");
    get_files(txt_files, &cnt_txt, "txt");

    printf("[client 1] записываю их в РОП...\n");
    write_in_shm(sem_id, adr, txt_files, cnt_txt, 1);
    
    printf("[client 1] получаю результаты обработки сервера...\n");
    get_results_from_shm(sem_id, adr, SEM_RESP_C1, cnt_txt, 1);

    printf("[client 1] отсоединяюсь от РОП...\n");
    time_t t = disconnect_shm(adr);
    printf("[client 1] время последнего отсоединения: %s", ctime(&t));

    return 0;
}