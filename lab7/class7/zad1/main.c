// Program w którym procesy macierzysty i potomny próbują odpowiednio spisywać i odczytywać i wypisywać na konsoli wartości z pamięci wspólnej
// (w nieskończonej pętli). Dostęp do pamięci powinien zostać zabezpieczony odpowiednio skonfigurowanym semaforem.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_SIZE 1024 // rozmiar pamięci współdzielonej
#define SEM_NAME "/mysemaphore" // nazwa semafora

int main() {
    int shmid;
    char *shmaddr;
    sem_t *sem;

    // Utwórz pamięć współdzieloną
    shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Przypisz pamięć współdzieloną do adresu procesu
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Utwórz semafor
    sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    // Utwórz proces potomny
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Proces potomny
        while (1) {
            sem_wait(sem); // Zablokuj semafor

            // Odczytaj wartość z pamięci współdzielonej i wypisz na konsoli
            int value = atoi(shmaddr);
            printf("Proces potomny odczytał wartość: %d\n", value);

            sem_post(sem); // Odblokuj semafor
            sleep(1);
        }
    } else {
        // Proces macierzysty
        int i = 0;
        while (1) {
            sem_wait(sem); // Zablokuj semafor

            // Zapisz wartość do pamięci współdzielonej
            sprintf(shmaddr, "%d", i);

            // Wypisz wartość na konsoli
            printf("Proces macierzysty zapisał wartość: %d\n", i);

            sem_post(sem); // Odblokuj semafor
            sleep(1);
            i++;
        }

        // Usuń semafor
        sem_unlink(SEM_NAME);

        // Odłącz pamięć współdzieloną
        if (shmdt(shmaddr) == -1) {
            perror("shmdt");
            exit(1);
        }

        // Usuń pamięć współdzieloną
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }
    }

    return 0;
}
