#include <stdio.h>
#include <zconf.h>
int main()
{
    // if(forkOut == 0){
    //     printf("Proces potomny: %d\n", forkOut);
    // }
    // else {
    //     printf("Proces macierzysty: %d\n", forkOut);
    // }

    // for (int i = 0; i < 10; i++)
    // {
    //     printf("Poczatek: %d\n", getpid());
    //     int forkOut = fork();
    //     printf("Koniec: %d\n", getpid());
    //     if (forkOut == 0){
    //         return 0;
    //     }
    // }

    int rpid = getpid();

    for (int i = 0; i < 10; i++)
    {
        printf("Poczatek: %d\n", getpid());
        if (getpid() == rpid)
        {
            fork();
        }
        printf("Koniec: %d\n", getpid());
    }



    return 0;
}