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
    key_t key;
    message_buf sbuf;
    message_buf rbuf;
    size_t buf_length;

    key = 1234;

    if ((msqid = msgget(key, msgflg)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    /* Paso 1: Envía "Are you hearing me?"*/
    sbuf.mtype = 1;
    (void)strcpy(sbuf.mtext, "Are you hearing me?");
    buf_length = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0)
    {
        printf("%d, %ld, %s, %ld\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }

    /* Paso 2: Recibe "Loud and clear" del Proceso 2*/
    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("%s\n", rbuf.mtext);

    /* Paso 3: Envía "I can hear you too"*/
    sbuf.mtype = 1;
    (void)strcpy(sbuf.mtext, "I can hear you too");
    buf_length = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0)
    {
        printf("%d, %ld, %s, %ld\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }

    exit(0);
}