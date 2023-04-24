
#include "zad2.h"

void mask()
{
    /*  Zamaskuj sygnał SIGUSR2, tak aby nie docierał on do procesu */
    sigset_t newmask;
    sigset_t oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        perror("Could not block the signal\n");
        return;
    }
    check_mask();
}

void process()
{

    /*  Stworz nowy process potomny i uruchom w nim program ./check_fork
        W procesie macierzystym:
            1. poczekaj 1 sekundę
            2. wyślij SIGUSR1 do procesu potomnego
            3. poczekaj na zakończenie procesu potomnego */
    
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Could not make a child process\n");
        return;
    }
    if (pid == 0)
    {
        if (execl("./check_fork.c", "check_fork", NULL) == -1){
            perror("Could not execute execl function\n");
            return;
        }
        
    }
    else
    {
        sleep(1);
        kill(pid, SIGUSR1);
        wait(NULL);
    }
}

int main()
{
    mask();
    process();

    return 0;
}