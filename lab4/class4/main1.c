#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// napisz program który wyśle sygnał sigusr1 do samego siebie (raise).
// Funkcją signal obsłuż sygnał w taki sposób że wydrukujesz na konsoli komunikat 
// typu "ja cie ale sygnał"

void handle_sigusr1(int sig)
{
    printf("Ja cię, ale sygnał!\n");
}

int main()
{
    // Zarejestruj obsługę sygnału SIGUSR1
    if (signal(SIGUSR1, handle_sigusr1) == SIG_ERR)
    {
        perror("Nie udało się zarejestrować obsługi sygnału");
        exit(1);
    }

    // Wyślij sygnał SIGUSR1 do samego siebie
    if (raise(SIGUSR1) != 0)
    {
        perror("Nie udało się wysłać sygnału");
        exit(1);
    }

    return 0;
}