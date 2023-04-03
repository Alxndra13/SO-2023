// napisz program który tworzy proces potomy a następnie w (nieskończonej) pętli wysyła do swojego potomka sygnały SIGUSR1.
// Potomek powinien zliczać otrzymane od przodka sygnały SIGUSER1 (i wyświetlać na bierząco stan licznika)
// użyj języka c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile int count = 0; // we use volatile variable to prevent optimization by the compiler

void child_handler(int signum) {
    count++;
    printf("Child received %d SIGUSR1 signals\n", count);
}

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {
        // this is the child process code
        signal(SIGUSR1, child_handler);

        while (1) {
            sleep(1);
        }
    }
    else {
        // this is the parent process code
        while (1) {
            kill(pid, SIGUSR1);
            sleep(0);
        }
    }

    return 0;
}
