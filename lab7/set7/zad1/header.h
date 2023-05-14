#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

// hairsalon simulation numbers
#define HAIRDRESSER_TOTAL 3 // hairdressers
#define CHAIR_TOTAL 2 // chairs
#define QUEUE_TOTAL 5 // waiting room  - queue
#define HAIRCUT_TOTAL 10 // number of hairstyles
#define BUFFER_SIZE 1024
#define CLIENT_TOTAL 10

// queue
char queue_pop(char*);
void queue_push(char*, char);
// bool queue_full(char*);
bool queue_empty(char*);

// library
typedef int Semaphore;
#define SEM_QUEUE_NAME "0"
#define SEM_CHAIRS_NAME "1"
#define SEM_HAIRDRESSERS_NAME "2"
#define SEM_BUFFER_NAME "3"
#define NAME getenv("HOME")
#define HAIRDRESSER_EXECUTABLE "./hairdresser"
#define CLIENT_EXECUTABLE "./client"

// memory
char* shared_memory_init(const char*, int);
int shared_memory_get(const char*, int);
void shared_memory_detach(char*);
void shared_memory_clean(const char*);

// semaphore
Semaphore sem_create(const char*, int);
Semaphore sem_open(const char*);
void sem_unlink(const char*);
void sem_wait_for(Semaphore);
void sem_release(Semaphore);