#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

int main()
{
    long long totalSize = 0;

    DIR *directory = opendir(".");
    if (directory == NULL)
    {
        printf("Fail in opening directory\n");
        return 1;
    }

    struct dirent *currentPoint;
    struct stat stats;

    while ((currentPoint = readdir(directory)) != NULL)
    {
        if (stat(currentPoint->d_name, &stats) != 0)
        {
            printf("Cannot get file info\n");
            continue;
        }
        if (!S_ISDIR(stats.st_mode))
        {
            printf("%ld %s\n", stats.st_size, currentPoint->d_name);
            totalSize += stats.st_size;
        }
    }

    closedir(directory);

    printf("Total size: %lld\n", totalSize);

    return 0;
}