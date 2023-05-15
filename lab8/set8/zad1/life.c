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

struct ThreadData {
    int row;
    int col;
    char *src;
    char *dst;
};

void* thread_worker(void *arg) {
    struct ThreadData *data = (struct ThreadData*) arg;
    int row = data->row;
    int col = data->col;
    char *src = data->src;
    char *dst = data->dst;
    printf("Hello from thread id: %d, i: %d, j: %d\n", pthread_self(), row, col);
    dst[row * GRID_WIDTH + col] = is_alive(row, col, src);
    return NULL;
}

void create_threads(struct ThreadData thread_data[][GRID_WIDTH], char *foreground, char *background) {
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            fflush(stdout);
            thread_data[i][j].row = i;
            thread_data[i][j].col = j;
            thread_data[i][j].src = foreground;
            thread_data[i][j].dst = background;
            pthread_create(&threads[i][j], NULL, thread_worker, &thread_data[i][j]);
        }
    }
}

void wait_for_threads() {
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        for (int j = 0; j < GRID_WIDTH; ++j) {
            pthread_join(threads[i][j], NULL);
        }
    }
}

int main() {
    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr(); // Start curses mode

    char *foreground = create_grid();
    char *background = create_grid();
    char *tmp;

    init_grid(foreground);

    struct ThreadData thread_data[GRID_HEIGHT][GRID_WIDTH];

    while (true) {
        // draw_grid(foreground);
        usleep(500 * 1000);

        create_threads(thread_data, foreground, background);
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
