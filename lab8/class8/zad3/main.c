// napisz program w C: napisz program w którym stworzysz n wątków (parametr programu)
// w którym każdy w nieskończonej pętli wypisuje na konsole napis typu "hello tu wątek n, iteracja m".
// dodatkowo po stworzeniu wszystkich wątków program główny także (w nieskończonej pętli)
// wypisuje na konsole napis typu "hello tu program główny iteracja m"

// zmodyfikuj poprzednie tak żeby program główny "zabija" w sposób asynchroniczny wszystkie wątki po wypisaniu na konsole swojego napisu 10 razy

// zmodyfikuj poprzednie w taki sposób, że w wątku masz mieć dwa miejsca drukowania komunikatu na konsole 
// następnie program główny zabija swoje wątki w sposób synchroniczny po wypisaniu n razy swojego komunikatu. 
// Ustaw punkt synchronizacji na miejsce pomiędzy wypisaniem pierwszego i drugiego komunikatu na konsole.



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS_NUMBER 20

pthread_mutex_t mutex;

void *thread_action(void *arg)
{
    int thread_id = *(int *)arg;
    int iteration = 0;

    while (1) {
        sleep(1);
        pthread_mutex_lock(&mutex); // Zablokowanie mutexu przed drukowaniem
        printf("thread %d, iteration %d\n", thread_id, iteration);
        pthread_mutex_unlock(&mutex); // odblokowanie
        iteration++;

        pthread_mutex_lock(&mutex);
        printf("second from thread %d, iteration %d\n", thread_id, iteration);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("Incorrect number of arguments. Expected one.\n");
        return 1;
    }

    int threads_number = atoi(argv[1]);

    pthread_t threads[threads_number];
    int threads_ids[threads_number];

    pthread_mutex_init(&mutex, NULL); // inicjalizacja mutexu

    for (int i = 0; i < threads_number; i++)
    {
        threads_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_action, &threads_ids[i]) != 0) // miejsce zapisu, dodatkowe opcje, funkcja akcji, argument funkcji akcji
        {
            perror("Error while creating a new thread.\n");
            return 1;
        }
    }

    int iteration = 0;
    while (iteration < 10) {
        sleep(1);
        pthread_mutex_lock(&mutex);
        printf("MAIN program, iteration %d\n", iteration);
        pthread_mutex_unlock(&mutex);
        iteration++;
    }

    for (int i=0; i<threads_number; i++){
        pthread_cancel(threads[i]);
    }

    return 0;
}