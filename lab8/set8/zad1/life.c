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

pthread_t threads[GRID_WIDTH][GRID_HEIGHT];

typedef struct ThreadData
{
    int row;
    int col;
    char *src;
    char *dst;
} ThreadData;

void handle_update(int signum, siginfo_t *info, void *abc)
{
    ThreadData *data = (ThreadData *)info->si_value.sival_ptr;
    data->dst[data->row * GRID_WIDTH + data->col] = is_alive(data->row, data->col, data->src);
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

void *thread_worker()
{
    while (true)
    {
        pause();
    }
}

void create_threads(char *foreground, char *background)
{

    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            pthread_create(&threads[i][j], NULL, thread_worker, NULL);
        }
    }
}

void update_threads(char *foreground, char *background)
{
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            ThreadData *thread_data = malloc(sizeof(ThreadData));
            thread_data->src = foreground;
            thread_data->dst = background;
            thread_data->row = i;
            thread_data->col = j;
            union sigval val;
            val.sival_ptr = (void *)thread_data;
            pthread_sigqueue(threads[i][j], SIGUSR1, val);
        }
    }
}

int main()
{
    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode
    set_signal_action();

    char *foreground = create_grid();
    char *background = create_grid();
    char *tmp;

    init_grid(foreground);
    create_threads(foreground, background);

    while (true)
    {
        draw_grid(foreground);
        usleep(500 * 1000);

        // create_threads(foreground, background);
        // wait_for_threads();
        update_threads(foreground, background);

        tmp = foreground;
        foreground = background;
        background = tmp;
    }

    endwin(); // End curses mode
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}
