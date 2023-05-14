#include "header.h"

int main()
{
    char *shared = shared_memory_init(NAME, BUFFER_SIZE);
    if (shared == NULL)
    {
        return -1;
    }
    shared[0] = '\0';



    sem_create(SEM_QUEUE_NAME, QUEUE_TOTAL);
    sem_create(SEM_CHAIRS_NAME, QUEUE_TOTAL);
    sem_create(SEM_HAIRDRESSERS_NAME, QUEUE_TOTAL);
    sem_create(SEM_BUFFER_NAME, QUEUE_TOTAL);

    printf("SIMULATION: Welcome to the hair salon!\n");
    fflush(stdout);
    for (int i = 0; i < HAIRDRESSER_TOTAL; ++i)
    {
        if (fork() == 0)
        {
            execl(HAIRDRESSER_EXECUTABLE, HAIRDRESSER_EXECUTABLE, NULL);
        }
    }

    for (int i = 0; i < CLIENT_TOTAL; i++)
    {
        if (fork() == 0)
        {
            execl(CLIENT_EXECUTABLE, CLIENT_EXECUTABLE, NULL);
        }
    }

    while(wait(NULL) > 0){};
    printf("SIMULATION: Everything done. Closing the salon.\n");


    sem_unlink(SEM_QUEUE_NAME);
    sem_unlink(SEM_HAIRDRESSERS_NAME);
    sem_unlink(SEM_CHAIRS_NAME);
    sem_unlink(SEM_BUFFER_NAME);
    printf("SIMULATION: Semaphores unlinked.\n");

    shared_memory_clean(NAME);

    printf("SIMULATION: memory cleaned.\n");


    return 0;
}