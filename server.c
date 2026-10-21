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
    int msgflg = IPC_CREAT | 0666;
    key_t key = 1234;
    message_buf sbuf, rbuf;

    if ((msqid = msgget(key, msgflg)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    // recibe del cliente 1 (mtype 1) y le responde por mtype 3
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("Cliente 1 dice: %s\n", rbuf.mtext);

    sbuf.mtype = 3;
    strcpy(sbuf.mtext, "Hola cliente 1, recibido");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, IPC_NOWAIT) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    // recibe del cliente 2 (mtype 2) y le responde por mtype 4
    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("Cliente 2 dice: %s\n", rbuf.mtext);

    sbuf.mtype = 4;
    strcpy(sbuf.mtext, "Hola cliente 2, recibido");
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, IPC_NOWAIT) < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    exit(0);
}