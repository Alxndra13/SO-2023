#include <stdio.h>
#include <string.h>
#include <time.h>
#include "libwc.h"
#include <regex.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

#define MAX_COMMAND_SIZE 2048

typedef enum
{
    INIT,
    COUNT,
    SHOW,
    DELETE,
    DESTROY,
    QUIT,
    INVALID
} COMMAND_TYPE;

regex_t initRgx;
regex_t countRgx;
regex_t showRgx;
regex_t deleteRgx;
regex_t destroyRgx;
regex_t quitRgx;

COMMAND_TYPE type;
int numInput;
bool wasInit = false;
bool replActive = true;
char path[MAX_COMMAND_SIZE];

void interpret_input(char *input, size_t inputSize)
{
    if (regexec(&initRgx, input, 0, NULL, 0) == 0)
    {
        type = INIT;
        sscanf(input, "init %d", &numInput);
    }
    else if (regexec(&countRgx, input, 0, NULL, 0) == 0)
    {
        type = COUNT;
        char *pathString = input + strlen("count ");
        int len = strlen(pathString);
        if (pathString[len - 1] == '\n')
        {
            pathString[len - 1] = '\0';
        }

        strcpy(path, pathString);
    }
    else if (regexec(&showRgx, input, 0, NULL, 0) == 0)
    {
        type = SHOW;
        sscanf(input, "show %d", &numInput);
    }
    else if (regexec(&deleteRgx, input, 0, NULL, 0) == 0)
    {
        type = DELETE;
        sscanf(input, "delete %d", &numInput);
    }
    else if (regexec(&destroyRgx, input, 0, NULL, 0) == 0)
    {
        type = DESTROY;
    }
    else if (regexec(&quitRgx, input, 0, NULL, 0) == 0)
    {
        type = QUIT;
    }
    else
    {
        type = INVALID;
    }

    if (type == INIT && numInput == 0)
    {
        type = INVALID;
        printf("REPL>> size has to be at least 1");
    }
}

void execute_input(BlockArray *array)
{
    switch (type)
    {
    case INIT:
        if (wasInit)
        {
            printf("REPL>> execute destroy before initializing again\n");
        }
        else
        {
            printf("REPL>> initializing\n");
            lib_init(array, numInput);
            wasInit = true;
        }
        break;
    case COUNT:
        printf("REPL>> counting\n");
        lib_fill(array, path);
        break;
    case SHOW:
        printf("REPL>> showing\n");
        printf("%s", lib_get(array, numInput));
        break;
    case DELETE:
        printf("REPL>> deleting\n");
        lib_pop(array, numInput);
        break;
    case DESTROY:
        printf("REPL>> destroying\n");
        lib_free(array);
        wasInit = false;
        break;
    case QUIT:
        printf("REPL>> exiting the repl\n");
        replActive = false;
        break;
    case INVALID:
        printf("REPL>> invalid command, try again\n");
    }
}

int main()
{
    // kompilacja regex
    regcomp(&initRgx, "init [0-9][0-9]*", 0);
    regcomp(&countRgx, "count ..", 0);
    regcomp(&showRgx, "show [0-9][0-9]*", 0);
    regcomp(&deleteRgx, "delete [0-9][0-9]*", 0);
    regcomp(&destroyRgx, "destroy", 0);
    regcomp(&quitRgx, "quit", 0);

    BlockArray *array = malloc(sizeof(BlockArray));
    char *input;
    size_t inputSize;

    struct timespec timespecStart, timespecEnd;
    struct tms tmsStart, tmsEnd;

    double real, user, system;

    while (replActive)
    {
        printf("REPL>> ");
        input = NULL;
        getline(&input, &inputSize, stdin);
        fflush(NULL);

        interpret_input(input, inputSize);
        fflush(NULL);

        clock_gettime(CLOCK_REALTIME, &timespecStart);
        times(&tmsStart);

        execute_input(array);

        times(&tmsEnd);
        clock_gettime(CLOCK_REALTIME, &timespecEnd);

        real = ((timespecEnd.tv_sec - timespecStart.tv_sec) + (timespecEnd.tv_nsec - timespecStart.tv_nsec)) / CLOCKS_PER_SEC;
        user = (tmsEnd.tms_utime - tmsStart.tms_utime) + (tmsEnd.tms_cutime - tmsStart.tms_cutime);
        system = (tmsEnd.tms_stime - tmsStart.tms_stime) + (tmsEnd.tms_cstime - tmsStart.tms_cstime);
        printf("Real time: %f ms\n", real);
        printf("User time: %f ticks\n", user);
        printf("System time: %f ticks\n", system);

        fflush(NULL);
        free(input);
    }
    return 0;
}