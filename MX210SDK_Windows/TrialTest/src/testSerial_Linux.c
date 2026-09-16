#ifndef _WIN32

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "mx210.h"

typedef struct
{
    MX210SerialPort_t port;
    volatile int isrunning;
} ThreadContext;

void* reader_thread(void* arg)
{
    ThreadContext* ctx = (ThreadContext*)arg;
    char readbuffer[4096];
    while (ctx->isrunning)
    {
        size_t read_bytes = MX210SerialPort_Read(ctx->port, readbuffer, sizeof(readbuffer) - 1, 10);
        
        if (read_bytes > 0)
        {
            readbuffer[read_bytes] = '\0';
            printf("\r");
            printf("%s", readbuffer);
            //printf("\n> ");
            fflush(stdout);
        }
    }
    return NULL;
}

int main(void)
{
    printf("\n Linux - MX210 Serial Communication \n");
    printf("====================================\n");

    MX210SerialPortsList ports = MX210SerialPort_GetPorts();
    if (ports.count == 0)
    {
        printf("No serial ports found.\n");
        return 1;
    }

    printf("Available Serial Ports:\n\n");

    for (size_t i = 0; i < ports.count; ++i)
    {
        printf("  %zu. %s\n", i + 1, ports.availableports[i]);
    }
    printf("\nSelect port: ");
    fflush(stdout);

    int userselection;
    if (scanf("%d", &userselection) != 1 || userselection < 1 || userselection >(int)ports.count)
    {
        fprintf(stderr, "Invalid userselection.\n");
        return 1;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    char selectedport[256];
    strncpy(selectedport, ports.availableports[userselection - 1], sizeof(selectedport) - 1);
    selectedport[sizeof(selectedport) - 1] = '\0';
    MX210SerialPort_FreePorts(&ports);

    MX210SerialPort_t serial = MX210SerialPort_Create();
    if (!serial)
    {
        fprintf(stderr, "Failed to create handle.\n");
        return 1;
    }
    if (!MX210SerialPort_Open(serial, selectedport, 921600))
    {
        fprintf(stderr, "Failed to open %s\n", selectedport);
        MX210SerialPort_Destroy(serial);
        return 1;
    }

    printf("\nOpening %s\n", selectedport);
    printf(">> Type 'exit' to quit.\n\n");

    ThreadContext ctx;
    ctx.port = serial;
    ctx.isrunning = 1;

    pthread_t reader;
    if (pthread_create(&reader, NULL, reader_thread, &ctx) != 0)
    {
        fprintf(stderr, "Failed to create a reader thread\n");
        MX210SerialPort_Close(serial);
        MX210SerialPort_Destroy(serial);
        return 1;
    }

    char mx210command[1024];
    printf(">> ");
    fflush(stdout);

    while (ctx.isrunning && MX210SerialPort_IsOpen(serial))
    {
        if (fgets(mx210command, sizeof(mx210command), stdin) == NULL)break;

        size_t len = strlen(mx210command);
        if (len > 0 && mx210command[len - 1] == '\n')
            mx210command[len - 1] = '\0';

        if (strcmp(mx210command, "exit") == 0)break;
        if (strlen(mx210command) == 0)
        {
            printf(">> ");
            fflush(stdout);
            continue;
        }
        strcat(mx210command, "\r\n");
        if (!MX210SerialPort_Write(serial, mx210command, strlen(mx210command)))
        {
            fprintf(stderr, "\nMX210 write failed.\n");
            break;
        }
        //printf(">> ");
        fflush(stdout);
    }

    ctx.isrunning = 0;
    pthread_join(reader, NULL);

    MX210SerialPort_Close(serial);
    MX210SerialPort_Destroy(serial);

    printf("\n%s is closed.\n", selectedport);

    return 0;
}

#endif