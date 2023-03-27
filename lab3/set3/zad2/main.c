#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Incorrect number of arguments. There's one required\n");
        return 1;
    }

    // wyłączenie buforowania danych wyjściowych -> unikamy problemu z wypisywaniem danych na wyjście po funkcji execl
    setbuf(stdout, NULL);

    printf("%s", argv[0]);

    // wywołanie /bin/ls
    if (execl("/bin/ls", "/bin/ls", argv[1], (char *)NULL) == -1)
    {
        printf("Error while execulting execl");
        return 1;
    }

    return 0;
}