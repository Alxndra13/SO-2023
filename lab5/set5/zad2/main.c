#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

double f(double x)
{
    return 4.0 / (x * x + 1.0);
}

double integral(double a, double b, double dx)
{
    double sum = 0.0;
    for (double x = a; x < b; x += dx)
    {
        sum += f(x) * dx;
    }
    return sum;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Incorrect number of arguments. Expected two.\n");
        return 1;
    }

    double dx = atof(argv[1]);
    int n = atoi(argv[2]);
    double a = 0.0;
    double b = 1.0;

    struct timespec time_start, time_end;
    clock_gettime(CLOCK_REALTIME, &time_start);

    int fd[n][2];

    // create pipes
    for (int i = 0; i < n; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            printf("Error while executing pipe\n");
            return 1;
        }
    }

    // create child processes
    for (int i = 0; i < n; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            printf("Fail while executing fork\n");
            return 1;
        }
        if (pid == 0)
        {
            close(fd[i][0]);
            double start = a + i * (b - a) / n;
            double end = a + (i + 1) * (b - a) / n;
            double result = integral(start, end, dx);
            write(fd[i][1], &result, sizeof(double));
            close(fd[i][1]);
            return 1;
        }
    }

    // wait for child processes to finish and collect results
    double sum = 0.0;
    for (int i = 0; i < n; i++)
    {
        double result;
        close(fd[i][1]);
        read(fd[i][0], &result, sizeof(double));
        close(fd[i][0]);
        sum += result;
        wait(NULL);
    }

    clock_gettime(CLOCK_REALTIME, &time_end);

    printf("Result: %f\n", sum);

    struct timespec difftime;
    if ((time_end.tv_nsec - time_start.tv_nsec) < 0){
        difftime.tv_sec = time_end.tv_sec - time_start.tv_sec-1;
        difftime.tv_nsec = 1000000000+time_end.tv_nsec-time_start.tv_nsec;
    }
    else {
        difftime.tv_sec = time_end.tv_sec - time_start.tv_sec;
        difftime.tv_nsec = time_end.tv_nsec - time_start.tv_nsec;
    }

    printf("Time: %lds %ldns\n", difftime.tv_sec, difftime.tv_nsec);

    return 0;
}