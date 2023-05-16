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

int main()
{
    pthread_mutex_init(&mutex);

    // tworzenie wątków
    for (int i = 0; i < THREAD_TOTAL; i++)
    {
        threads_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_worker, &threads_ids[i]) != 0)
        {
            perro("Error while creating threads\n");
        }
    }

    return 0;
}