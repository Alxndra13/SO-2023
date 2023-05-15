#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "grid.h"

typedef struct ThreadData
{
    int thread_number;
    char *src;
    char *dst;
} ThreadData;

pthread_t threads[THREAD_TOTAL];
pthread_barrier_t barrier;
char *foreground;
char *background;

void *thread_worker(void *arg)
{
    int thread_number = *(int*) arg;
    int cells_per_thread = GRID_HEIGHT * GRID_WIDTH / THREAD_TOTAL;
    int start = thread_number * cells_per_thread;
    int end = (thread_number + 1) * cells_per_thread;
    while (true)
    {
        for (int cell = start; cell < end; cell++)
        {
            int i = cell % GRID_WIDTH;
            int j = cell / GRID_HEIGHT;
            background[i * GRID_WIDTH + j] = is_alive(i, j, foreground);
        }
        pthread_barrier_wait(&barrier);
    }

    return NULL;
}

void create_threads(char *foreground, char *background)
{
    for (int i = 0; i < THREAD_TOTAL; ++i)
    {
        int *thread_number = malloc(sizeof(int));
        *thread_number = i;
        pthread_create(&threads[i], NULL, thread_worker, (void *)thread_number);
    }
}

int main()
{
    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode

    foreground = create_grid();
    background = create_grid();
    init_grid(foreground);
    char *tmp;

    pthread_barrier_init(&barrier, NULL, THREAD_TOTAL + 1);
    create_threads(foreground, background);

    while (true)
    {
        draw_grid(foreground);
        usleep(500 * 1000);

        tmp = foreground;
        foreground = background;
        background = tmp;

        pthread_barrier_wait(&barrier);
    }

    endwin(); // End curses mode
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}
