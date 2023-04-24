#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#define HOME_PATH getenv("HOME")
#define MESSAGE_MAX_SIZE 512
#define SERVER_ID 1
#define CLIENTS_MAX 10

typedef struct Message
{
    long type;
    key_t key;
    int client_id;
    int other_id;
    char content[MESSAGE_MAX_SIZE];
    struct tm time_struct;
} Message;

typedef enum MessageType
{
    INIT = 1,
    LIST = 2,
    TOONE = 3,
    TOALL = 4,
    STOP = 5
} MessageType;

const int MESSAGE_SIZE = sizeof(Message);