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
    void* ethernetroute;
    volatile int isrunning;
} ThreadContext;

void* reader_thread(void* arg)
{
    ThreadContext* ctx = (ThreadContext*)arg;
    char readbuffer[4096], answerBuffer[4096];
    while (ctx->isrunning)
    {
        memset(readbuffer, 0, sizeof(readbuffer));
        int received = MX210TcpIP_readReceivedData((MX210TcpIp_t*)ctx->ethernetroute, MX210_PORT_8000, readbuffer, 
            sizeof(readbuffer));
        if (received)
        {
            while (MX210TcpIP_readLine(readbuffer, sizeof(readbuffer), answerBuffer, sizeof(answerBuffer)))
                printf("%s", answerBuffer);
            fflush(stdout);
        }
    }
    return 0;
}


int main(void)
{
    MX210TcpIp_t* tcpIpDevice;
    int i;
    char answerBuffer[4096], buffer[4096];

    printf("\n Linux - MX210 TCP/IP Communication \n");
    printf("====================================\n");
    printf("\n\n2. TCP/IP Port 8000 Demo:\n");

    tcpIpDevice = MX210TcpIP_init("192.168.0.111");

    if (!MX210TcpIP_connect(tcpIpDevice, MX210_PORT_8000))
    {
        printf("PORT 8000 connected!\n");

    }

    ThreadContext ctx;
    ctx.ethernetroute = tcpIpDevice;
    ctx.isrunning = 1;

    pthread_t reader;
    if (pthread_create(&reader, NULL, reader_thread, &ctx) != 0)
    {
        fprintf(stderr, "Failed to create a reader thread\n");
        MX210TcpIP_disconnect(tcpIpDevice, MX210_PORT_8000);
        MX210TcpIP_deinit(tcpIpDevice);
        return 1;
    }

    printf(">> Type 'exit' to for Demo 2.\n\n");
    char mx210command[1024];
    printf(">> ");
    fflush(stdout);

    while (ctx.isrunning && MX210TcpIP_isConnected(tcpIpDevice, MX210_PORT_8000))
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
        if (!MX210TcpIP_send(tcpIpDevice, MX210_PORT_8000, mx210command, strlen(mx210command)))
        {
            fprintf(stderr, "\nMX210 write failed.\n");
            break;
        }
        //printf("> ");
        fflush(stdout);
    }
    ctx.isrunning = 0;
    pthread_join(reader, NULL);
        
    MX210TcpIP_disconnect(tcpIpDevice, MX210_PORT_8000);
    printf("PORT 8000 disconnected\n");

    printf("\n\n2. TCP/IP Port 8005 Demo:\n");
    getchar();

    while (1)
    {
        if (MX210TcpIP_isConnected(tcpIpDevice, MX210_PORT_8005))
        {
            MX210TcpIP_readReceivedData(tcpIpDevice, MX210_PORT_8005, buffer, sizeof(buffer));

            while (MX210TcpIP_readLine(buffer, sizeof(buffer), answerBuffer, sizeof(answerBuffer)))
                printf("%s", answerBuffer);
        }
        else
        {
            if (!MX210TcpIP_connect(tcpIpDevice, MX210_PORT_8005)) printf("PORT 8005 reconnected\n");
            MX210TcpIP_clearReceiveBuffer(tcpIpDevice, MX210_PORT_8005);
        }
    }

    MX210TcpIP_disconnect(tcpIpDevice, MX210_PORT_8005);
    MX210TcpIP_deinit(tcpIpDevice);

    return 0;
}

#endif