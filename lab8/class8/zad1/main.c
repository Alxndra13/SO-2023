// napisz program w C: napisz program w którym stworzysz n wątków (parametr programu)
// w którym każdy w nieskończonej pętli wypisuje na konsole napis typu "hello tu wątek n, iteracja m".
// dodatkowo po stworzeniu wszystkich wątków program główny także (w nieskończonej pętli)
// wypisuje na konsole napis typu "hello tu program główny iteracja m"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_THREADS_NUMBER 20

void *thread_action(void *arg)
{
    int thread_id = *(int *)arg;
    int iteration = 0;

    while (1) {
        sleep(1);
        printf("Hello, here's thread %d, iteration %d\n", thread_id, iteration);
        iteration++;
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
    while (1) {
        sleep(1);
        printf("Hello, here's the main program, iteration %d\n", iteration);
        iteration++;
    }

    return 0;
}