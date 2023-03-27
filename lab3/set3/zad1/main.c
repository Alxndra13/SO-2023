#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <zconf.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Incorrect number of arguments. There's one required\n");
        return 1;
    }

    int childrenNumber = atoi(argv[1]);

    for (int i = 0; i < childrenNumber; i++)
    {
        pid_t forkOut = fork();
        if (forkOut == -1)
        {
            printf("Fail in creating child proccess\n");
            return 1;
        }
        if (forkOut == 0)
        {
            printf("Parent: %d, child: %d\n", getppid(), getpid());
            return 0;
        }
    }

    for (int i = 0; i < childrenNumber; i++)
    {
        wait(NULL);
    }

    printf("argv[1]: %s\n", argv[1]);

    return 0;
}