// Napisz program w którym uruchomisz n wątków (parametr programu) realizujących te same funkcje.
// W ramach swojego działania wątki mają inkrementować wartość zmiennej globalnej i wypisywać jej waetość na konsole wraz z informacją
// identyfikującą wątek dokonujący wypisania.
// Dostęp do zmiennej zabezpiecz mutexem
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_TOTAL 5

int global_variable = 0;
pthread_mutex_t mutex;
pthread_t threads[THREAD_TOTAL];
int threads_ids[THREAD_TOTAL];

void *thread_worker(void *arg)
{
    int thread_id = *(int *) arg;
    pthread_mutex_lock(&mutex);
    global_variable++;
    printf("Thread id: %d, global variable = %d\n", thread_id, global_variable);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main()
{
    pthread_mutex_init(&mutex, NULL);

    // tworzenie wątków
    for (int i = 0; i < THREAD_TOTAL; i++)
    {
        threads_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_worker, &threads_ids[i]) != 0)
        {
            perror("Error while creating threads\n");
            exit(1);
        }
    }

    // oczekiwanie na zakończenie wątków
    for (int i = 0; i < THREAD_TOTAL; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Error while joining threads\n");
            exit(1);
        }
    }
    
    pthread_mutex_destroy(&mutex);

    return 0;
}