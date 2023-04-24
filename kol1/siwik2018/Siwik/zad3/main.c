#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i, pid;

    if (argc != 2)
    {
        printf("Not a suitable number of program arguments");
        exit(2);
    }
    else
    {
        for (i = 0; i < atoi(argv[1]); i++)
        {
            //*********************************************************
            // Uzupelnij petle w taki sposob aby stworzyc dokladnie argv[1] procesow potomnych, bedacych dziecmi
            //   tego samego procesu macierzystego.
            // Kazdy proces potomny powinien:
            // - "powiedziec ktorym jest dzieckiem",
            //-  jaki ma pid,
            //- kto jest jego rodzicem
            //******************************************************
            pid = fork();
            if (pid < 0)
            {
                perror("Fork\n");
                return 1;
            }
            if(pid == 0){
                printf("Dziecko nr %d, PID %d, rodzic to %d\n", i, getpid(), getppid());
                exit(0);
            }
            else{
                wait(NULL);
            }
        }
    }
    return 0;
}
