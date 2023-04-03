// Przetestuj działanie trzech wybranych flag w funkcji sigation.
// Jedną z nich powinna być flaga SA_SIGINFO. Dla tej flagi zainstaluj procedurę obsługi sygnału (handler)
// dla odpowiednio dobranych sygnałów stosując składnie procedury handlera z trzema argumentami.
// Wypisz i skomentuj (przygotowując odpowiednie scenariusze) trzy różne informacje, a dodatkowo także
// numer sygnału oraz identyfikator PID procesu wysyłającego dostarczane w strukturze siginfo_t przekazywanej jako drugi argument funkcji handlera.
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// obsługa sygnałów
// przyjmuje signal - numer sygnału, si - wskaźnik na strukture siginfo_t, unused - nieużywany argument
void handler(int signal, siginfo_t *si, void *unused)
{
}

int main(int argc, char** argv)
{
    if (argc != 1) {
        printf("Program does not take any arguments.\n");
        return 1;
    }

    // struktura do zarejestrowania procedury obsługi sygnałów dla wybranych sygnałów
    struct sigaction action;

    printf("Flag: SA_SIGINFO");
    

    act.sa_sigaction = handler; // funkcja obsługi sygnałów na handler
    sigemptyset(&act.sa_mask);  // czyszczenie zbioru sygnałów, które są blokowane podczas obsługi (nic nie blokujemy)
    act.sa_flags = SA_SIGINFO;  // drugi argument funkcji handler będzie wskaźnikiem na siginfo_t a nie na int

    return 0;
}