#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>
long long totalSize = 0;

int file_info (const char *path, const struct stat *stats, int typeflag){
    if (typeflag == FTW_F){
        printf("%ld %s\n", stats->st_size, path);
        totalSize += stats->st_size;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Incorrect number of arguments");
        return 1;
    }

    char* directoryName = argv[1];

    if(ftw(directoryName, file_info, 1) == -1){
        printf("Fail in executing ftw function");
        return 1;
    }

    printf("Total size: %lld\n", totalSize);

    return 0;
}