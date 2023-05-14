#include "header.h"

int main(void)
{
    char *queue = shared_memory_init(NAME, BUFFER_SIZE);
    if (queue == NULL)
        return -1;

    Semaphore sem_queue;
    Semaphore sem_chairs;
    Semaphore sem_hairdressers;
    Semaphore sem_buffer;

    sem_queue = sem_open(SEM_QUEUE_NAME);
    sem_chairs = sem_open(SEM_CHAIRS_NAME);
    sem_hairdressers = sem_open(SEM_HAIRDRESSERS_NAME);
    sem_buffer = sem_open(SEM_BUFFER_NAME);

    printf("Hairdresser pid: %d arrived\n", getpid());
    fflush(stdout);

    while (1)
    {
        sem_wait_for(sem_hairdressers);

        sem_wait_for(sem_buffer);
        char haircut = queue_pop(queue);
        sem_release(sem_buffer);

        printf("Hairdresser pid: %d is working on the haircut %d\n", getpid(), haircut);
        fflush(stdout);
        usleep(1000);
        printf("Hairdresser pid: %d finished working on the haircut %d\n", getpid(), haircut);
        fflush(stdout);

        sem_release(sem_chairs);
        sem_release(sem_queue);

        if (queue_empty(queue))
        {
            usleep(1000000);
            if (queue_empty(queue))
                break;
        }
    }

    printf("Hairdresser pid: %d has empty queue. He is leaving.\n", getpid());
    fflush(stdout);

    shared_memory_detach(queue);
    return 0;
}