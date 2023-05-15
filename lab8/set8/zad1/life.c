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

const int num_threads = GRID_WIDTH * GRID_HEIGHT;
pthread_t threads[GRID_WIDTH][GRID_HEIGHT];

typedef struct ThreadData
{
    int row;
    int col;
    char *src;
    char *dst;
} ThreadData;

void *thread_worker(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    data->dst[data->row * GRID_WIDTH + data->col] = is_alive(data->row, data->col, data->src);
    return NULL;
}

void create_threads(char *foreground, char *background)
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
            pthread_create(&threads[i][j], NULL, thread_worker, (void *)thread_data);
        }
    }
}

void wait_for_threads()
{
    for (int i = 0; i < GRID_HEIGHT; ++i)
    {
        for (int j = 0; j < GRID_WIDTH; ++j)
        {
            pthread_join(threads[i][j], NULL);
        }
    }
}

int main()
{
    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode

    char *foreground = create_grid();
    char *background = create_grid();
    char *tmp;

    init_grid(foreground);

    while (true)
    {
        draw_grid(foreground);
        usleep(500 * 1000);

        create_threads(foreground, background);
        wait_for_threads();

        tmp = foreground;
        foreground = background;
        background = tmp;
    }

    endwin(); // End curses mode
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}
