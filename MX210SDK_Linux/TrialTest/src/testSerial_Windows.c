#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <process.h>

#include "mx210.h"

typedef struct
{
    MX210SerialPort_t serialport;
    volatile int isrunning;
}ThreadContext;

DWORD WINAPI reader_thread(LPVOID arg)
{
    ThreadContext* ctx = (ThreadContext*)arg;
    char readbuffer[4093];
    while (ctx->isrunning)
    {
        size_t readbytes = MX210SerialPort_Read(ctx->serialport, readbuffer, sizeof(readbuffer) - 1, 10);
        if (readbytes > 0)
        {
            readbuffer[readbytes] = '\0';
            printf("\r");
            printf("%s", readbuffer);
            fflush(stdout);
        }
    }
    return 0;
}

int main(void)
{
    printf("\n Win - M210 Serial Communication \n");
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
    ctx.serialport = serial;
    ctx.isrunning = 1;
    HANDLE reader = CreateThread(
        NULL,
        0,
        reader_thread,
        &ctx,
        0,
        NULL
    );

    if (reader == NULL)
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
        if (len > 0 && mx210command[len - 1] == '\n')mx210command[len - 1] = '\0';

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
    WaitForSingleObject(reader, INFINITE);
    CloseHandle(reader);

    MX210SerialPort_Close(serial);
    MX210SerialPort_Destroy(serial);

    printf("\n%s is closed.\n", selectedport);

    return 0;
}