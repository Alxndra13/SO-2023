#include "header.h"
#include <stdio.h>

// Queue
char queue_pop(char *queue)
{
    if (strlen(queue) == 0) // if the queue is empty we return 0 to indicate that there's nothing to be popped
    {
        return -1;
    }
    char element = queue[0];                     // element being popped from the queue
    memcpy(queue, queue + 1, strlen(queue) + 1); // shift all element in queue to the left
    return element;
}

void queue_push(char *queue, char value)
{
    if (QUEUE_TOTAL == strlen(queue)) // we dont put anything into the queue, ignore new element
    {
        exit(0);
    }
    size_t size = strlen(queue); // current size of the queue
    queue[size] = value; // put element to the end of the queue
    queue[size+1] = 0;
}

bool queue_empty(char *queue)
{
    return strlen(queue) == 0;
}

// Memory
char *shared_memory_init(const char *name, int size)
{
    int shmid = shared_memory_get(name, size); // retrieve a shared memory identifier
    if (shmid == -1)
    {
        perror("Error while initiating shared memory - shared_memory_get failed\n");
        return NULL;
    }
    void *shmaddr = shmat(shmid, NULL, 0); // attach to the shared memory segment
    if (shmaddr == (void *)-1)
    {
        perror("Error while initiating shared memory - shmat failed\n");
        return NULL;
    }

    return (char *)shmaddr;
}

int shared_memory_get(const char *name, int size) // retrieve an existing shared memory segment or create a new one
{
    key_t key = ftok(name, 0);
    if (key == -1)
    {
        perror("Error while getting shared memory - ftok failed\n");
        exit(1);
    }
    int shmid = shmget(key, size, 0644 | IPC_CREAT); // retrieve the shared memory segment associated with the key
    if (shmid == -1)
    {
        perror("Error while getting shared memory - shmget failed\n");
        exit(1);
    }
    return shmid;
}

void shared_memory_detach(char *shmaddr)
{
    if (shmdt(shmaddr) == -1)
    { // detach the calling process from the shared memory segment specified by the shmaddr
        perror("Error while detaching shared memory - shmdt failed\n");
        exit(1);
    }
}

void shared_memory_clean(const char *name) // clean up and remove a shared memory segment
{
    int shmid = shared_memory_get(name, 0);
    if (shmid == -1)
    {
        perror("Error while cleaning shared memory - shared_memory_get failed\n");
        exit(1);
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    { // remove the shared memory segment
        perror("Error while cleaning shared memory - shmctl failed\n");
        exit(1);
    }
}

// Semaphore
Semaphore sem_create(const char *name, int value)
{
    key_t key = ftok(NAME, name[0]); // generate a unique key for interprocess communication
    if (key == -1)
    {
        perror("Error while creating semaphore - ftok failed\n");
        exit(1);
    }
    Semaphore semid = semget(key, 1, 0664 | IPC_CREAT); // create a set of semaphores with 1 semaphore
    if (semid == -1)
    {
        perror("Error while creating semaphore - semget failed\n");
        exit(1);
    }
    if (semctl(semid, 0, SETVAL, value) == -1) // set the initial value of the semaphore in the semaphore set
    {
        perror("Error while creating semaphore - semctl failed\n");
        exit(1);
    }
    return semid; // semaphore set identifier
}

Semaphore sem_open(const char *name) // open an existing semaphore
{
    key_t key = ftok(NAME, name[0]);
    if (key == -1)
    {
        perror("Error while opening semaphore - ftok failed\n");
        exit(1);
    }
    Semaphore semid = semget(key, 1, 0); // access an existing semaphore set
    if (semid == -1)
    {
        perror("Error while opening semaphore - semget failed\n");
        exit(1);
    }
    return semid;
}

void sem_unlink(const char *name)
{
    Semaphore semid = sem_open(name); // open semaphore to be unlinked
    if (semid == -1)
    {
        perror("Error while unlinking semaphore - sem_open failed\n");
        exit(1);
    }
    if (semctl(semid, 0, IPC_RMID) == -1) //  remove semaphore set associated with the semaphore set identifier, deleting the semaphore
    {
        perror("Error while unlinking sempahore - semctl failed\n");
        exit(1);
    }
}

void sem_wait_for(Semaphore semid) // "wait" operation on a semaphore
{
    struct sembuf operation; // structure to specify the operation to be performed on the semaphore
    operation.sem_num = 0;   // semaphore position in the set of semaphores
    operation.sem_op = -1;   // operation to be done, -1 - wait (decrement the value of semaphore by 1)
    operation.sem_flg = 0;   // no special flags

    if (semop(semid, &operation, 1) == -1) // perform the operation
    {
        perror("Error while wait operation on semaphore - semop failed\n");
        exit(1);
    }
}

void sem_release(Semaphore semid)
{
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = 1; // 1 - release (increment the value of semaphore by 1)
    operation.sem_flg = 0;

    if (semop(semid, &operation, 1) == -1)
    {
        perror("Error while release operation on semaphore - semop failed\n");
        exit(1);
    }
}