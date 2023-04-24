#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>

#include "header.h"

int first_id = 0;
int squeue;
key_t client_queues[CLIENTS_MAX];

void message_logs(Message *message)
{
    struct tm time_measure = message->time_struct;
    FILE *file = fopen("message_logs.txt", "a");
    switch (message->type)
    {
    case INIT:
        if (message->client_id == -1)
            fprintf(file, "INIT: Max number of clients was reached.\n");
        else
            fprintf(file, "INIT: ClientID: %d.\n", message->client_id);
        break;
    case LIST:
        fprintf(file, "LIST: ClientID: %d.\n", message->client_id);
        break;
    case TOALL:
        fprintf(file, "2ALL: %s. ClientID: %d.\n", message->content, message->client_id);
        break;
    case TOONE:
        fprintf(file, "2ONE: %s. SenderID: %d, ReceiverID: %d.\n", message->content, message->client_id, message->other_id);
        break;
    case STOP:
        fprintf(file, "STOP: ClientID: %d.\n", message->client_id);
        break;
    default:
        printf("ERROR");
        exit(1);
    }

    fprintf(file, "Time: %02d:%02d:%02d\n", time_measure.tm_hour, time_measure.tm_min, time_measure.tm_sec);
    fclose(file);
}

void init(Message *message)
{
    while (client_queues[first_id] != -1 && first_id < CLIENTS_MAX - 1)
    {
        first_id++;
    }
    if (client_queues[first_id] != -1 && first_id == CLIENTS_MAX - 1)
    {
        message->client_id = -1;
    }
    else
    {
        message->client_id = first_id;
        client_queues[first_id] = message->key;
        if (first_id < CLIENTS_MAX - 1)
        {
            first_id++;
        }
    }
    int queue_id = msgget(message->key, 0);
    msgsnd(queue_id, message, MESSAGE_SIZE, 0);
    message_logs(message);
}

void stop(int client_id)
{
    client_queues[client_id] = -1;

    if (client_id < first_id)
    {
        first_id = client_id;
    }
}

void end()
{
    Message *message = malloc(sizeof(Message));
    for (int i = 0; i < CLIENTS_MAX; i++)
    {
        if (client_queues[i] != -1)
        {
            message->type = STOP;
            int queue_id = msgget(client_queues[i], 0);
            msgsnd(queue_id, message, MESSAGE_SIZE, 0);
            msgrcv(squeue, message, MESSAGE_SIZE, STOP, 0);
        }
    }
    msgctl(squeue, IPC_RMID, NULL);
    exit(0);
}

void list(int client_id)
{
    Message *newMessage = malloc(sizeof(Message));
    strcpy(newMessage->content, "");
    for (int i = 0; i < CLIENTS_MAX; i++)
    {
        if (client_queues[i] != -1)
        {
            sprintf(newMessage->content + strlen(newMessage->content), "ID %d is running.\n", i);
        }
    }

    newMessage->type = LIST;
    int queue_id = msgget(client_queues[client_id], 0);
    msgsnd(queue_id, newMessage, MESSAGE_SIZE, 0);
}

void toall(Message *message)
{
    for (int i = 0; i < CLIENTS_MAX; i++)
    {
        if (client_queues[i] != -1 && i != message->client_id)
        {
            int other_queue_id = msgget(client_queues[i], 0);
            msgsnd(other_queue_id, message, MESSAGE_SIZE, 0);
        }
    }
}

void toone(Message *message)
{
    int other_queue_id = msgget(client_queues[message->other_id], 0);
    msgsnd(other_queue_id, message, MESSAGE_SIZE, 0);
}

int main()
{
    for (int i = 0; i < CLIENTS_MAX; i++)
    {
        client_queues[i] = -1;
    }

    key_t key = ftok(HOME_PATH, SERVER_ID);
    squeue = msgget(key, IPC_CREAT | 0666);

    signal(SIGINT, end);

    Message *message = malloc(sizeof(Message));
    while (1)
    {
        msgrcv(squeue, message, MESSAGE_SIZE, -6, 0);
        switch (message->type)
        {
        case INIT:
            init(message);
            break;
        case LIST:
            list(message->client_id);
            message_logs(message);
            break;
        case TOALL:
            toall(message);
            message_logs(message);
            break;
        case TOONE:
            toone(message);
            message_logs(message);
            break;
        case STOP:
            stop(message->client_id);
            message_logs(message);
            break;
        default:
            perror("Incorrect message type\n");
        }
    }
    free(message);
}