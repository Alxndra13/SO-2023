#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#include "header.h"

key_t key;
int queue_id;
int squeue_id;
int client_id;

void measure_time(Message *message){
    time_t time_measure = time(NULL);
    message->time_struct = *localtime(&time_measure);
}

int init()
{
    Message *message = malloc(MESSAGE_SIZE);
    measure_time(message);
    message->type = INIT;
    message->key = key;

    msgsnd(squeue_id, message, MESSAGE_SIZE, 0);
    msgrcv(queue_id, message, MESSAGE_SIZE, 0, 0);

    if (message->client_id == -1)
    {
        perror("Client limit has been reached. Leaving.\n");
        exit(0);
    }
    return message->client_id;
}

void stop()
{
    Message *message = malloc(MESSAGE_SIZE);
    measure_time(message);

    message->type = STOP;
    message->key = key;

    msgsnd(squeue_id, message, MESSAGE_SIZE, 0);
    msgctl(queue_id, IPC_RMID, NULL);
    exit(0);
}

void receive_server_message()
{
    Message *message = malloc(MESSAGE_SIZE);
    while (msgrcv(queue_id, message, MESSAGE_SIZE, 0, IPC_NOWAIT) >= 0)
    {
        if (message->type == STOP)
        {
            printf("Received stop message. Leaving.\n");
            stop();
        }
        else
        {
            printf("Time of message from %d: %02d:%02d:%02d:\n%s\n",
                   message->client_id, message->time_struct.tm_hour, message->time_struct.tm_min,
                   message->time_struct.tm_sec, message->content);
        }
    }
}

void list()
{
    Message *message = malloc(MESSAGE_SIZE);
    measure_time(message);
    message->type = LIST;
    message->client_id = client_id;

    msgsnd(squeue_id, message, MESSAGE_SIZE, 0);
    msgrcv(queue_id, message, MESSAGE_SIZE, 0, 0);
    printf("%s\n", message->content);
}

void toall(char *to_send)
{
    Message *message = malloc(MESSAGE_SIZE);
    measure_time(message);
    message->type = TOALL;
    strcpy(message->content, to_send);

    message->client_id = client_id;

    msgsnd(squeue_id, message, MESSAGE_SIZE, 0);
}

void toone(int destination_id, char *to_send)
{
    Message *message = malloc(sizeof(Message));
    measure_time(message);

    message->type = TOONE;
    message->client_id = client_id;
    message->other_id = destination_id;

    strcpy(message->content, to_send);

    msgsnd(squeue_id, message, MESSAGE_SIZE, 0);
}

int main()
{
    srand(time(NULL));
    key = ftok(HOME_PATH, rand() % 255 + 1);
    queue_id = msgget(key, IPC_CREAT | 0666);
    key_t squeue_key = ftok(HOME_PATH, SERVER_ID);
    squeue_id = msgget(squeue_key, 0);

    signal(SIGINT, stop);

    client_id = init();
    size_t size = 0;
    ssize_t read;
    char *command = NULL;

    while (1)
    {
        receive_server_message();

        printf("Command: ");
        read = getline(&command, &size, stdin);
        command[read - 1] = '\0';

        receive_server_message();

        if (strcmp(command, "") == 0)
            continue;

        char *curr_command = strtok(command, " ");
        if (strcmp(curr_command, "LIST") == 0)
        {
            list();
        }
        else if (strcmp(curr_command, "2ALL") == 0)
        {
            curr_command = strtok(NULL, " ");
            if (curr_command == NULL)
            {
                printf("Incorrect argument.\n");
                continue;
            }
            char *message = curr_command;
            toall(message);
        }
        else if (strcmp(curr_command, "2ONE") == 0)
        {
            curr_command = strtok(NULL, " ");
            if (curr_command == NULL)
            {
                printf("Incorrect argument.\n");
                continue;
            }
            int destination_id = atoi(curr_command);
            curr_command = strtok(NULL, " ");
            char *message = curr_command;
            toone(destination_id, message);
        }
        else if (strcmp(curr_command, "STOP") == 0)
        {
            stop();
        }
        else
        {
            printf("Incorrect command.\n");
        }
    }

    return 0;
}