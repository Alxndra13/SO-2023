#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include "grid.h"

typedef struct ThreadData
{
    int thread_number;
} ThreadData;

pthread_t threads[THREAD_TOTAL];
char *foreground;
char *background;
pthread_barrier_t barrier;

void handle_update(int signum, siginfo_t *info, void *abc)
{
    ThreadData *data = (ThreadData *)info->si_value.sival_ptr;
    int cells_per_thread = GRID_HEIGHT * GRID_WIDTH / THREAD_TOTAL;
    int start = data->thread_number * cells_per_thread;
    int end = (data->thread_number + 1) * cells_per_thread;
    for (int cell = start; cell < end; cell++)
    {
        int i = cell % GRID_WIDTH;
        int j = cell / GRID_HEIGHT;
        background[i * GRID_WIDTH + j] = is_alive(i, j, foreground);
    }
    free(data);
}

void set_signal_action()
{
    struct sigaction action;
    action.sa_sigaction = handle_update;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &action, NULL);
}

void *thread_worker(void *arg)
{
    while (true)
    {
        pause();
    }
    pthread_barrier_wait(&barrier);

}

void create_threads()
{
    for (int i = 0; i < THREAD_TOTAL; ++i)
    {
        pthread_create(&threads[i], NULL, thread_worker, NULL);
    }
}

void update_threads()
{
    for (int i = 0; i < THREAD_TOTAL; ++i)
    {
        ThreadData *thread_data = malloc(sizeof(ThreadData));
        thread_data->thread_number = i;
        union sigval val;
        val.sival_ptr = (void *)thread_data;
        pthread_sigqueue(threads[i], SIGUSR1, val);
    }
}

int main()
{
    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode
    set_signal_action();

    foreground = create_grid();
    background = create_grid();
    char *tmp;

    init_grid(foreground);
    pthread_barrier_init(&barrier, NULL, THREAD_TOTAL + 1);
    create_threads();

    while (true)
    {
        draw_grid(foreground);
        usleep(500 * 1000);

        update_threads();

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
