#ifndef _LIBWC_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> //obsługa bool
#include <string.h>

typedef struct
{
    char **array;   // tablica stringów
    size_t size;    // rozmiar struktury
    bool *isActive; // tablica boolowa - czy dany indeks jest zapisany
    size_t actualSize;
} BlockArray;

// 1
BlockArray lib_create(size_t size);

// 2
void lib_fill(BlockArray *blockArray, char *inputName);

// 3
char *lib_get(BlockArray *blockArray, size_t index);

// 4
void lib_pop(BlockArray *blockArray, size_t index);

// 5
void lib_free(BlockArray *blockArray);

#endif