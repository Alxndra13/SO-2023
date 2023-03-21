#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
typedef struct
{
    char toFind;
    char toPut;
    char *inputFilename;
    char *outputFilename;
} Arguments;

bool check_input(int argc, char *argv[], Arguments *args)
{
    if (argc != 5)
    {
        printf("Incorrect number of arguments. Expected 4 of them\n");
        return false;
    }

    if (strlen(argv[1]) > 1)
    {
        printf("You need to input only one ASCII sign to find\n");
        return false;
    }

    if (strlen(argv[2]) > 1)
    {
        printf("You need to input only one ASCII sign to replace with it\n");
        return false;
    }

    args->toFind = argv[1][0];
    args->toPut = argv[2][0];
    args->inputFilename = argv[3];
    args->outputFilename = argv[4];
    return true;
}

void replace_chars(Arguments *args, size_t inputSize, char *block)
{
    for (size_t i = 0; i < inputSize; i++)
    {
        if (block[i] == args->toFind)
        {
            block[i] = args->toPut;
        }
    }
}

void lib_replace(Arguments *args)
{
    FILE *inputFile = fopen(args->inputFilename, "r");
    if (!inputFile)
    {
        printf("Fail in opening input file\n");
        return;
    }

    FILE *outputFile = fopen(args->outputFilename, "w");
    if (!outputFile)
    {
        printf("Fail in opening output file\n");
        return;
    }

    fseek(inputFile, 0, SEEK_END);
    size_t inputSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    char *block = calloc(inputSize, sizeof(char));
    fread(block, sizeof(char), inputSize, inputFile);

    replace_chars(args, inputSize, block);

    fwrite(block, sizeof(char), inputSize, outputFile);

    fclose(inputFile);
    fclose(outputFile);
    free(block);
}

void sys_replace(Arguments *args)
{
    int inputFile = open(args->inputFilename, O_RDONLY);
    if (inputFile < 0)
    {
        printf("Fail in opening input file");
    }

    int outputFile = open(args->outputFilename, O_WRONLY | O_CREAT, S_IRUSR, S_IWUSR);
    if (outputFile < 0)
    {
        printf("Fail in opening output file");
    }

    long inputSize = lseek(inputFile, 0, SEEK_END);
    lseek(inputFile, 0, SEEK_SET);

    char *block = calloc(inputSize, sizeof(char));
    read(inputFile, block, inputSize * sizeof(char));

    replace_chars(args, inputSize, block);

    write(outputFile, block, inputSize * sizeof(char));

    close(inputFile);
    close(outputFile);
    free(block);
}

time_t time_result(struct timespec timespecStart, struct timespec timespecEnd)
{
    if (((timespecEnd.tv_sec - timespecStart.tv_sec) + (timespecEnd.tv_nsec - timespecStart.tv_nsec)) / CLOCKS_PER_SEC < 0)
    {
        return (timespecEnd.tv_sec - timespecStart.tv_sec + 1000000000 + timespecEnd.tv_nsec - timespecStart.tv_nsec) / CLOCKS_PER_SEC;
    }
    return ((timespecEnd.tv_sec - timespecStart.tv_sec) + (timespecEnd.tv_nsec - timespecStart.tv_nsec)) / CLOCKS_PER_SEC;
}

// WARIANT LIB
int main(int argc, char *argv[])
{
    Arguments *args = malloc(sizeof(Arguments));
    if (!check_input(argc, argv, args))
    {
        return 0;
    };

    struct timespec timespecStart, timespecEnd;
    clock_gettime(CLOCK_REALTIME, &timespecStart);

#ifdef USE_LIB
    lib_replace(args);
#else
    sys_replace(args);
#endif
    clock_gettime(CLOCK_REALTIME, &timespecEnd);
    printf("TIME: %ld ms\n", time_result(timespecStart, timespecEnd));

    return 0;
}