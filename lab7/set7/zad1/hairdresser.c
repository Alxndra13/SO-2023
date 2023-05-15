#include "header.h"

int main(void)
{
    char *queue = shared_memory_init(NAME, BUFFER_SIZE);
    if (queue == NULL)
        return -1;

    Semaphore sem_chairs = sem_open(SEM_CHAIRS_NAME);
    Semaphore sem_buffer = sem_open(SEM_BUFFER_NAME);

    printf("Hairdresser pid: %d arrived\n", getpid());
    fflush(stdout);

    while (1)
    {
        sem_wait_for(sem_chairs);

        char haircut = queue_pop(queue);
        if (haircut == -1)
        {
            if (queue_empty(queue))
            {
                sleep(1);
                if (queue_empty(queue))
                    break;
            }
        }

        printf("Hairdresser pid: %d is working on the haircut %d\n", getpid(), haircut);
        fflush(stdout);
        sleep(haircut); // the bigger the haircut number, the longer it takes
        printf("Hairdresser pid: %d finished working on the haircut %d\n", getpid(), haircut);
        fflush(stdout);

        sem_release(sem_buffer);

        if (queue_empty(queue))
        {
            sleep(1);
            if (queue_empty(queue))
                break;
        }
    }

    printf("Hairdresser pid: %d sees empty queue. He is leaving.\n", getpid());
    fflush(stdout);

    sem_release(sem_chairs);

    shared_memory_detach(queue);
    return 0;
}