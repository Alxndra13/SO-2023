#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <zconf.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PATH_SIZE 2555
int traverse(char *path, char *string)
{
    DIR *directory = opendir(path); //opening directory
    if (directory == NULL)
    {
        perror("Error while opening directory\n");
        return 1;
    }

    struct dirent *current; //current directory entry point
    while ((current = readdir(directory)) != NULL) //read the next directory entry from DIR *directory -> returns dirent pointer
    {
        if (strcmp(current->d_name, ".") == 0 || strcmp(current->d_name, "..") == 0)
        {
            continue; //don't go back
        }

        char currentPath[PATH_MAX];
        snprintf(currentPath, PATH_MAX, "%s/%s", path, current->d_name);

        struct stat stats; //information about the file
        if (stat(currentPath, &stats) == -1) //stat couldnt get info
        {
            perror("Error while executing stat function\n");
            return 1;
        }

        if (S_ISDIR(stats.st_mode)) //is a directory
        {
            pid_t forkOut = fork();
            if (forkOut == -1)
            {
                perror("Fail in creating child proccess\n");
                return 1;
            }
            if (forkOut == 0)
            {
                traverse(currentPath, string);
                return 0;
            }
        }

        if (S_ISREG(stats.st_mode)) //is a regular file
        {
            FILE *file = fopen(currentPath, "r"); //file opening
            if (file == NULL)
            {
                perror("Error while opening file\n");
                return 1;
            }

            char buffer[MAX_PATH_SIZE];
            if (fread(buffer, sizeof(char), strlen(string), file) <= 0) {
                perror("Error while reading file into buffer\n");
                return 1;
            }

            if (strcmp(buffer, string) == 0)
            {
                printf("%s/%s %d\n", path, current->d_name, getpid());
            }

            if (fclose(file) == EOF)
            {
                perror("Error while closing file\n");
                return 1;
            }
        }
    }
    if (closedir(directory) == -1)
    {
        perror("Error while closing directory\n");
        return 1;
    }

    return 0;
}

int check_input(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Incorrect number of arguments. Two are expected\n");
        return 1;
    }

    if (strlen(argv[2]) > MAX_PATH_SIZE)
    {
        printf("String cannot be longer than 255 characters\n");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (check_input(argc, argv) != 0)
    {
        return 1;
    }
    char *path = argv[1];
    char *string = argv[2];

    if (traverse(path, string) != 0)
    {
        return 1;
    }

    return 0;
}