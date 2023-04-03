#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define MAX_MODES 5

bool done = true;
int mode = 1;
int mode_count = 0;

void print_numbers()
{
    for (int i = 1; i <= 100; i++)
    {
        printf("CATCHER: %d\n", i);
    }
}

void print_time()
{
    time_t now = time(NULL);
    printf("CATHER: %s", ctime(&now));
}

void print_mode_count()
{
    printf("CATCHER: Number of operations done: %d\n", mode_count);
}

void print_time_loop(double measuredTime)
{
    static double acc = 1.0;
    acc += measuredTime;
    if (acc >= 1.0)
    {
        print_time();
        acc = 0.0;
    }
}

void signal_handler_exit(int signum)
{
    printf("CATCHER: Exiting\n");
    fflush(NULL);
    exit(0);
}

void handler(int signo, siginfo_t *info, void *context)
{
    int pid = info->si_pid;
    int request = info->si_status;

    if (request < 1 || request > MAX_MODES) //is the received mode valid
    {
        printf("CATCHER: Invalid mode %d\n", request);
    }
    else
    {
        mode_count += 1;
        mode = request;
        done = false;
    }

    kill(pid, SIGUSR1); //send SIGUSR1 back to the sender, indicating it's been handled
}

int main()
{
    // set up the signal handler
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handler;
    action.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &action, NULL);

    clock_t startTime = clock();
    clock_t endTime = clock();

    printf("CATCHER: Starting, PID: %d\n", getpid());
    while (1)
    {
        double measuredTime = (double)(endTime - startTime) / CLOCKS_PER_SEC; //measure the operation time
        startTime = clock();

        if (done)
        {
            continue;
        }

        switch (mode)
        {
        case 1:
            print_numbers();
            break;
        case 2:
            print_time();
            break;
        case 3:
            print_mode_count();
            break;
        case 4:
            print_time_loop(measuredTime);
            break;
        case 5:
            signal_handler_exit(SIGINT);
            break;
        default:
            break;
        }

        if (mode != 4)
        {
            done = true;
        }

        endTime = clock();
    }

    return 0;
}