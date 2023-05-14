#include "header.h"

int main(void)
{
    srand(getpid());

    Semaphore sem_queue;
    Semaphore sem_chairs;
    Semaphore sem_hairdressers;
    Semaphore sem_buffer;

    char *queue = shared_memory_init(NAME, BUFFER_SIZE);
    if (queue == NULL)
        exit(1);

    sem_queue = sem_open(SEM_QUEUE_NAME);
    sem_chairs = sem_open(SEM_CHAIRS_NAME);
    sem_hairdressers = sem_open(SEM_HAIRDRESSERS_NAME);
    sem_buffer = sem_open(SEM_BUFFER_NAME);
    printf("Client pid: %d arrived.\n", getpid());
    fflush(stdout);

    if (strlen(queue) >= QUEUE_TOTAL)
    {
        printf("Client pid: %d sees full queue. They are leaving.\n", getpid());
        fflush(stdout);
        return EXIT_SUCCESS;
    }

    sem_wait_for(sem_queue);

    sem_wait_for(sem_buffer);
    char haircut = (char)(rand() % HAIRCUT_TOTAL);

    printf("Client pid: %d chooses haircut %d.\n", getpid(), haircut);
    fflush(stdout);
    queue_push(queue, haircut);
    sem_release(sem_buffer);

    sem_release(sem_hairdressers);
    sem_wait_for(sem_chairs);

    printf("Client pid: %d is done. They are leaving.\n", getpid());
    fflush(stdout);

    shared_memory_detach(queue);

    return 0;
}
// #include "common.h"

// int main(void) {
//   srand(getpid());

//   Semaphore squeue;
//   Semaphore schairs;
//   Semaphore sbarbers;
//   Semaphore sbuffer;

//   char* queue = init_shared(NAME, BUFF_SZ);
//   if (queue == NULL)
//     return -1;

//   squeue = sema_open(SQUEUE_NAME);
//   schairs = sema_open(SCHAIRS_NAME);
//   sbarbers = sema_open(SBARBERS_NAME);
//   sbuffer = sema_open(SBUFFER_NAME);

//   if (strlen(queue) >= P) {
//     printf("client named %d can see the queue is full so they are leaving.\n",
//            getpid());
//     fflush(stdout);
//     return EXIT_SUCCESS;
//   }

//   sema_wait_for(squeue);

//   sema_wait_for(sbuffer);
//   char haircut = (char)(rand() % 128);

//   printf("client named %d wants the haircut %d \n", getpid(), haircut);
//   fflush(stdout);
//   queue_push(queue, haircut);
//   sema_release(sbuffer);

//   sema_release(sbarbers);
//   sema_wait_for(schairs);

//   printf("client named %d is done, good job\n", getpid());
//   fflush(stdout);

//   detach_shared(queue);

//   return EXIT_SUCCESS;
// }
