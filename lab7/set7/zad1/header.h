#include <stdbool.h>

// hairsalon simulation numbers
#define HAIRDRESSER_TOTAL 3 // hairdressers
#define CHAIR_TOTAL 2 // chairs
#define QUEUE_TOTAL 5 // waiting room  - queue
#define HAIRCUT_TOTAL 4 // number of hairstyles
#define BUFFER_SIZE 1024

// queue
char queue_pop(char*);
void queue_push(char*, char);
bool queue_full(char*);
bool queue_empty(char*);

// library
typedef int Semaphore;
#define SQUEUE_NAME "0"
#define SCHAIRS_NAME "1"
#define SBARBERS_NAME "2"
#define SBUFFER_NAME "3"
#define NAME getenv("HOME")
#define BARBER_EXECUTABLE "./barber"
#define CLIENT_EXECUTABLE "./client"

// memory
char* shared_memory_init(const char*, int);
bool shared_memory_detach(char*);
bool shared_memory_clean(const char*);

// semaphore
Semaphore sem_create(const char*, int);
Semaphore sem_open(const char*);
void sem_close(Semaphore);
void sem_unlink(const char*);
void sem_wait_for(Semaphore);
void sem_release(Semaphore);