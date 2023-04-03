#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_CALLS 4


int call_id;
int call_depth;


typedef void (handler_t)(int, siginfo_t, void*);


void set_action(struct sigaction action, handler_t handler, int flag) {
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = handler;
    action.sa_flags = flag;
    sigaction(SIGUSR1, &action, NULL);
    call_id = 0;
    call_depth = 0;
}


void info_handler(int signo, siginfo_t* info, void* context) {
    int INFORMATIONS_NUMBER = 5;

    char* descriptions[] = {
        "Signal number",
        "Process ID   ",
        "User ID      ",
        "Band event   ",
        "Signal value "
    };
    int values[] = {
        info->si_signo,
        info->si_pid,
        info->si_uid,
        info->si_band,
        info->si_value.sival_int
    };

    for (int i=0; i<INFORMATIONS_NUMBER; i++)
        printf(" - %s: %d \n", descriptions[i], values[i]);
}


void siginfo_test(struct sigaction action) {
    printf(" parent:\n");
    set_action(action, info_handler, SA_SIGINFO);
    kill(getpid(), SIGUSR1);
    printf("\n");

    printf(" child:\n");
    if (fork() == 0) {
        kill(getpid(), SIGUSR1);
        exit(0);
    }
    else
        wait(NULL);
    printf("\n");

    sigval_t sig_val;
    sig_val.sival_int = 2137;
    printf(" custom value %d\n", sig_val.sival_int);
    set_action(action, info_handler, SA_SIGINFO);
    sigqueue(getpid(), SIGUSR1, sig_val);
    // kill(getpid(), SIGUSR1);
}

void depth_handler(int signo, siginfo_t* info, void* context) {
    printf(" - new call:   (id = %d, depth = %d)\n", call_id, call_depth);
    
    call_id++;
    call_depth++;
    if (call_id < MAX_CALLS)
        kill(getpid(), SIGUSR1);
    call_depth--;

    printf(" - call ended: (id = %d, depth = %d)\n", call_id-1, call_depth);
}



void nodefer_test(struct sigaction action) {
    set_action(action, depth_handler, SA_NODEFER);
    kill(getpid(), SIGUSR1);
}


void resethand_test(struct sigaction action) {
    set_action(action, depth_handler, SA_RESETHAND);
    kill(getpid(), SIGUSR1);
    kill(getpid(), SIGUSR1);
}

int main(int argc, char** argv) {
    if (argc != 1) {
        fprintf(stderr, "Invalid arguments.\n");
        exit(1);
    }

    struct sigaction action;

    printf("\nFlag SIGINFO:\n");
    siginfo_test(action);
    printf("\n");


    printf("\nFlag RESETHAND:\n");
    resethand_test(action);

    printf("\nFlag NODEFER:\n");
    nodefer_test(action);
    printf("\n");
    return 0;
}