#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define FIRST_MODE 2; // number i in argv[i] means where is the mode to be done in argv[]
volatile int state = FIRST_MODE; // volatile -> it can be changed later

void handler() //increment the state when receiving signal from catcher
{
    state++;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("SENDER: Incorrect number of arguments, at least two are expected\n");
        return 1;
    }

    int mode;
    bool fail = false;

    int catcher_pid = atoi(argv[1]); // first argument -> catcher pid

    for (int i = 2; i < argc; i++) // we don't want to iterate from first argument - catcher pid
    {
        fail = false;
        mode = atoi(argv[i]); //current work mode to send to catcher

        //setup signal handler for received SIGUSR1 from catcher using sigaction()
        struct sigaction action;
        sigemptyset(&action.sa_mask);
        action.sa_handler = handler;
        sigaction(SIGUSR1, &action, NULL);

        sigval_t sigVal = {mode}; //contain the value of mode to be sent to catcher
        sigqueue(catcher_pid, SIGUSR1, sigVal); //send to catcher
        printf("SENDER: Sent mode %d\n", mode);

        time_t startTime = clock();
        while (state <= i){
            if(clock()-startTime > CLOCKS_PER_SEC*5){ //if the waiting time is more than 5 second, try 
                printf("SENDER: No confirmation, failed, retrying\n");
                fail = true;
                i--; //retry sending the signal that failed to receive information
                break;
            }
        }

        if(!fail){ //if there was no fail, sender prints that it received confirmation
            printf("SENDER: Received confirmation.\n");
        }
    }

    return 0;
}