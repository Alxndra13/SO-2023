#include "header.h"

int main(void)
{
    srand(getpid());

    char *queue = shared_memory_init(NAME, BUFFER_SIZE);
    if (queue == NULL)
        exit(1);

    Semaphore sem_queue = sem_open(SEM_QUEUE_NAME);
    Semaphore sem_chairs = sem_open(SEM_CHAIRS_NAME);
    Semaphore sem_hairdressers = sem_open(SEM_HAIRDRESSERS_NAME);
    Semaphore sem_buffer = sem_open(SEM_BUFFER_NAME);

    printf("Client pid: %d arrived. The queue length is %ld\n", getpid(), strlen(queue));
    fflush(stdout);
    
    if (strlen(queue) >= QUEUE_TOTAL)
    {
        printf("Client pid: %d sees full queue. They are leaving.\n", getpid());
        fflush(stdout);
        exit(0);
    }

    sem_wait_for(sem_queue);
    // printf("QUEUE: %d\n", strlen(queue));

    sem_wait_for(sem_buffer);
    
    char haircut = (char)(rand() % HAIRCUT_TOTAL + 1); // 1 to 10

    printf("Client pid: %d chooses haircut %d.\n", getpid(), haircut);
    fflush(stdout);
    queue_push(queue, haircut);
    sem_release(sem_buffer);

    sem_release(sem_hairdressers);
    sem_wait_for(sem_chairs);

    printf("Client pid: %d is done. They are leaving.\n", getpid());
    fflush(stdout);

    shared_memory_detach(queue);

    exit(0);
}