#ifndef __LIBWC_H__
#define __LIBWC_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    char **array; 
    size_t size;
    bool *isActive;
    size_t actualSize;
} BlockArray;

BlockArray* lib_create(size_t size);

void lib_init(BlockArray *BlockArray, size_t size);

void lib_fill(BlockArray *blockArray, char *inputName);

char *lib_get(BlockArray *blockArray, size_t index);

void lib_pop(BlockArray *blockArray, size_t index);

void lib_free(BlockArray *blockArray);

#endif