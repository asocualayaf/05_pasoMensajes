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
    key_t key;
    message_buf rbuf;
    message_buf sbuf;
    size_t buf_length;

    key = 1234;

    if ((msqid = msgget(key, 0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    /* Paso 1: Recibe "Are you hearing me?" del Proceso 1 */
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("%s\n", rbuf.mtext);

    /* Paso 2: Envía "Loud and clear" */
    sbuf.mtype = 2;
    (void)strcpy(sbuf.mtext, "Loud and clear");
    buf_length = strlen(sbuf.mtext) + 1;

    if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0)
    {
        printf("%d, %ld, %s, %ld\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
        exit(1);
    }

    /* Paso 3: Recibe "I can hear you too" del Proceso 1 */
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("%s\n", rbuf.mtext);

    exit(0);
}