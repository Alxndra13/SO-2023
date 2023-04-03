// Napisz program demonstrujący, czy ustawienia dyspozycji dla sygnałów, ich maski oraz czekające sysgnały są dziedziczone po wykonaniu funkcji fork oraz exec.
// W szczególności eksperymenty proszę wykonać dla sygnału SIGUSR1 w następujący sposób:

// Dziedziczenie ustawień sygnałów po wykonaniu funkcji fork. Proszę napisać program, który w zależności od wartości argumentu z linii poleceń,
// który może przyjmować wartości ignore, handler, mask lub pending, odpowiednio w procesie przodka ustawia ignorowanie,
// instaluje handler obsługujący sygnał wypisujący komunikat o jego otrzymaniu, maskuje ten sygnał
//  oraz sprawdza (przy zamaskowaniu tego sygnału) czy wiszący/oczekujący sygnał jest widoczny w procesie,
// a następnie przy pomocy funkcji raise wysyła sygnał do samego siebie oraz wykonuje odpowiednie dla danej opcji działania,
// po czym tworzy potomka funkcją fork i ponownie przy pomocy funkcji raise potomek wysyła sygnał do samego siebie
// (z wyjątkiem opcji pending, gdzie testowane jest sprawdzenie, czy sygnał czekający w przodku jest widoczny w potomku).

// Dziedziczenie ustawień sygnałów po wykonaniu funkcji exec. W podobny sposób sprawdź jaki wpływ na ustawienia sygnałów ma wywołanie funkcji exec.
// Rozpatrz opcje:  ignore, mask i pending.

// Przygotuj plik raport2.txt w którym nastąpi podsumowanie z wnioskami z wykonanych powyższych eksperymentów
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void sigusr1_handler(int signum) //message to be printed when signal is received
{
    printf("Received SIGUSR1 signal\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Incorrect number of arguments, one was expected\n");
        return 1;
    }

    char* taskType = argv[1];

    int isParent = 1;

    sigset_t set, oldSet; // creating the signal sets
    sigemptyset(&set); // initilize set to be empty
    sigaddset(&set, SIGUSR1); // //add SIGUSR1 to set


    if (strcmp(taskType, "ignore") == 0)
    {
        signal(SIGUSR1, SIG_IGN); //SIG_IGN -> signal will be ignored
    }
    else if (strcmp(taskType, "handler") == 0)
    {
        signal(SIGUSR1, sigusr1_handler);
    }
    else if (strcmp(taskType, "mask") == 0) //use the sigusr1_handler function
    {
        sigprocmask(SIG_BLOCK, &set, &oldSet); // set is added to te process signal mask, previous to be stored in oldSet
    }
    else if (strcmp(taskType, "pending") == 0)
    {   
        sigpending(&set); // get the list of signals that are currently pending
        if (sigismember(&set, SIGUSR1)) //is SIGUSR1 a member of the set
        {
            printf("Parent process: SIGUSR1 signal is pending\n");
        }
        else
        {
            printf("Parent process: SIGUSR1 signal is not pending\n");
        }
    }
    else
    {
        printf("Invalid task type\n");
        return 1;
    }

    raise(SIGUSR1); // send signal to itself

    pid_t forkOut = fork(); //create a child process

    if (forkOut < 0)
    {
        perror("Error in executing fork function\n");
        return 1;
    }
    if (forkOut == 0) //is current process the child process
    {
        raise(SIGUSR1); // child send signal to itself

        if (strcmp(taskType, "pending") == 0)
        {
            sigpending(&set); // get the list of signals that are currently pending
            if (sigismember(&set, SIGUSR1)) //is SIGUSR1 a member of the set
            {
                printf("Child process: SIGUSR1 signal is pending\n");
            }
            else
            {
                printf("Child process: SIGUSR1 signal is not pending\n");
            }
        }

        return 0;
    }
    wait(NULL); // wait for child process to terminate

    if (strcmp(taskType, "mask") == 0)
    {
        sigprocmask(SIG_SETMASK, &oldSet, NULL); //restore the process signal mask to its previous state using oldSet
    }

    return 0;
}