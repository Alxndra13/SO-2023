#include "zad3.h"
#include <string.h>

void readwrite(int pd, size_t block_size);

void createpipe(size_t block_size)
{
    /* Utwórz potok nienazwany */
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Error while executing pipe fuction\n");
        return;
    }

    /* Odkomentuj poniższe funkcje zamieniając ... na deskryptory potoku */
    check_pipe(fd);
    check_write(fd, block_size, readwrite);
}

void readwrite(int write_pd, size_t block_size)
{
    /* Otworz plik `unix.txt`, czytaj go po `block_size` bajtów
    i w takich `block_size` bajtowych kawałkach pisz do potoku `write_pd`.*/
    FILE *file = fopen("unix.txt", "r");
    fseek(file, 0L, SEEK_END);
    size_t bytes_read = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char buffer[block_size];

    size_t read_size = block_size;

    while (bytes_read > 0)
    {
        memset(buffer, 0, block_size); // wyczyszczenie go na wszelki
        if (bytes_read < block_size)
        {
            read_size = bytes_read;
        }
        fread(buffer, sizeof(char), read_size, file);
        write(write_pd, buffer, read_size * sizeof(char));
        bytes_read -= read_size;
    }

    /* Zamknij plik */
    fclose(file);
    close(write_pd);
}

int main()
{
    srand(42);
    size_t block_size = rand() % 128;
    createpipe(block_size);

    return 0;
}