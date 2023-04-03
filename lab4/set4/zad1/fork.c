#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

pid_t forkOut = 1;

void handler(int signum) // message to be printed when signal is received
{
    printf("PID: %d signal received\n", getpid());
}

void raise_signal() //raise a SIGUSR1 and print the pid of current process
{
    printf("PID: %d signal raising\n", getpid());
    raise(SIGUSR1);
}

void block_signal() //block the SIGUSR1 using sigprocmask
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);
}

void handle_pending() //check if there is a pending SIGUSR1
{
    sigset_t pendingSignals;
    sigpending(&pendingSignals);
    if (sigismember(&pendingSignals, SIGUSR1))
    {
        printf("PID: %d signal pending\n", getpid());
    }
    else
    {
        printf("PID: %d signal not pending\n", getpid());
    }
}

bool process(char *taskType)
{
    if (strcmp(taskType, "ignore") == 0)
    {
        signal(SIGUSR1, SIG_IGN); // SIG_IGN -> signal will be ignored
        raise_signal();
    }
    else if (strcmp(taskType, "handler") == 0)
    {
        signal(SIGUSR1, handler);
        raise_signal();
    }
    else if (strcmp(taskType, "mask") == 0 || strcmp(taskType, "pending") == 0)
    {
        signal(SIGUSR1, handler);
        if (forkOut > 0)
        {
            block_signal();
            raise_signal();
        }

        if (strcmp(taskType, "pending") == 0)
        {
            handle_pending();
        }
    }

    else
    {
        printf("Invalid task type\n");
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Incorrect number of arguments, one was expected\n");
        return 1;
    }

    char *taskType = argv[1];
    if (!process(taskType)) //false if there was invalid tasktype
    {
        return 1;
    }
    fflush(NULL);

    forkOut = fork(); // create a child process
    if (forkOut < 0)
    {
        perror("Error in executing fork function\n");
        return 1;
    }
    if (forkOut == 0)
    {
        if (!process(taskType))
        {
            return 1;
        }
    }
    else
    {
        wait(NULL);
    }

    fflush(NULL);

    return 0;
}