#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MSGSZ 128

typedef struct msgbuf
{
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main()
{
    int msqid;
    key_t key = 1234;
    message_buf sbuf, rbuf;

    if ((msqid = msgget(key, 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    // envia al server con tipo 2
    sbuf.mtype = 2;
    strcpy(sbuf.mtext, "Hola servidor, soy el cliente 2");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, IPC_NOWAIT) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    // lee solo su respuesta (tipo 4)
    if (msgrcv(msqid, &rbuf, MSGSZ, 4, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("Server: %s\n", rbuf.mtext);

    exit(0);
}