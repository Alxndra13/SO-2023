#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sighandler(int signum, siginfo_t *info, void *context)
{
    printf("Received signal %d with value %d\n", signum, info->si_value.sival_int);
}

int main(int argc, char *argv[])
{

    if (argc != 3)
    {
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;
    action.sa_flags = SA_SIGINFO;

    //..........
    // execute sigaction - set sighandler function to handle sigusr1
    if (sigaction(SIGUSR1, &action, NULL) < 0)
    {
        perror("Error while executing sigaction function\n");
        return 1;
    }

    int child = fork();
    if (child == 0)
    {
        // zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        // zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        // na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
        sigset_t mask;
        sigfillset(&mask);         // add all signals to mask
        sigdelset(&mask, SIGUSR1); // remove SIGUSR1 from mask
        if (sigprocmask(SIG_SETMASK, &mask, NULL) < 0)
        {
            perror("Error while executing sigprocmask function\n");
            return 1;
        }

        if(sigaction(SIGUSR1, &action, NULL) < 0) {
            perror("Error while executing sigaction function\n");
            return 1;
        }
        wait(NULL);
    }
    else
    {
        // wyslij do procesu potomnego sygnal przekazany jako argv[2]
        // wraz z wartoscia przekazana jako argv[1]
        int signal_num = atoi(argv[2]);
        int value = atoi(argv[1]);

        union sigval sv;
        sv.sival_int = value;
        if(sigqueue(child, signal_num, sv) < 0){
            perror("Error while executing sigqueue function\n");
            return 1;
        }
    }

    return 0;
}
