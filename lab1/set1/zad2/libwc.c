#include "libwc.h"

#define MAX_COMMAND_SIZE 2048

int result;

BlockArray *lib_create(size_t size)
{
    BlockArray *blockArray = malloc(sizeof(BlockArray));
    blockArray->array = calloc(size, sizeof(char *));
    blockArray->size = size;
    blockArray->isActive = calloc(size, sizeof(bool));
    blockArray->actualSize = 0;
    return blockArray;
}

void lib_init(BlockArray *blockArray, size_t size)
{
    blockArray->array = calloc(size, sizeof(char *));
    blockArray->size = size;
    blockArray->isActive = calloc(size, sizeof(bool));
    blockArray->actualSize = 0;
}

void lib_fill(BlockArray *blockArray, char *inputName)
{

    char tmpName[] = "wc_out_XXXXXX";
    int tmpFile = mkstemp(tmpName);

    // sprawdzenie czy sie utworzył
    if (tmpFile < 0)
    {
        printf("failed in creating a temporary file\n");
        return;
    }

    char wc_command[MAX_COMMAND_SIZE];
    snprintf(wc_command, MAX_COMMAND_SIZE, "wc %s 1> %s", inputName, tmpName);

    system(wc_command);

    // rozmiar pliku
    FILE *file = fopen(tmpName, "r");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // zawartość pliku
    char *block = calloc(fileSize, sizeof(char));
    fread(block, sizeof(char), fileSize, file);
    fclose(file);

    // usunięcie pliku
    snprintf(wc_command, MAX_COMMAND_SIZE, "rm -f '%s'", tmpName);
    system(wc_command);

    if (strlen(block) == 0)
    {
        printf("fail in reading input file\n");
        return;
    }

    if (blockArray->actualSize >= blockArray->size)
    {
        printf("not enough memory in blockArray\n");
        return;
    }
    blockArray->array[blockArray->actualSize] = block;
    blockArray->isActive[blockArray->actualSize] = true;
    blockArray->actualSize += 1;
}

char *lib_get(BlockArray *blockArray, size_t index)
{
    if (index >= blockArray->actualSize)
    {
        printf("index out of range\n");
        return "";
    }

    if (blockArray->isActive[index] == false)
    {
        printf("index is empty\n");
        return "";
    }
    return blockArray->array[index];
}

void lib_pop(BlockArray *blockArray, size_t index)
{
    if (index >= blockArray->actualSize)
    {
        printf("index out of range\n");
        return;
    }
    if (blockArray->isActive[index] == false)
    {
        printf("index is empty\n");
        return;
    }
    blockArray->isActive[index] = false;
    free(blockArray->array[index]);
}

void lib_free(BlockArray *blockArray)
{
    for (size_t index = 0; index < blockArray->actualSize; index++)
    {
        if (blockArray->isActive[index])
        {
            free(blockArray->array[index]);
        }
    }
    free(blockArray->array);
    free(blockArray->isActive);
}