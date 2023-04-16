#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COMMAND_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 4)
    {
        printf("Incorrect number of arguments. Expected 1 or 3\n");
        return 1;
    }
    char command[MAX_COMMAND_SIZE];
    if (argc == 2)
    {
        if (strcmp(argv[1], "nadawca") == 0)
        {
            sprintf(command, "mail -H | sort -k 2");
        }
        else if (strcmp(argv[1], "data") == 0)
        {
            sprintf(command, "mail -H | sort -k 3");
        }
        else {
            printf("Incorrect argument. Expected nadawca or data\n");
            return 1;
        }
        FILE * file = popen(command, "r");
        if (file == NULL) {
            printf("Error while executing mail\n");
            return 1;
        }
        char line[MAX_COMMAND_SIZE];
        while (fgets(line, MAX_COMMAND_SIZE, file) != NULL) {
            printf("%s", line);
        }
        pclose(file);
        
    }
    else{
        sprintf(command, "echo '%s' | mail -s '%s' '%s'", argv[3], argv[2], argv[1]);
        int status = system(command);
        if (status != 0) {
            printf("Error while sending mail\n");
            return 1;
        }
        printf("Mail sent\n");
    }
    return 0;
}
