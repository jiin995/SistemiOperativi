#include "procedure.h"
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void Wait_Sem(int id_sem,int n_sem)
{
    struct sembuf sem_buf;
        sem_buf.sem_num=n_sem;
        sem_buf.sem_flg=0;
		sem_buf.sem_op=-1;
    semop(id_sem,&sem_buf,1);
}

void Signal_Sem(int id_sem,int n_sem)
{
    struct sembuf sem_buf;
        sem_buf.sem_num=n_sem;
        sem_buf.sem_flg=0;
        sem_buf.sem_op=1;
    semop(id_sem,&sem_buf,1);
}

void buffer_init(buffer * buf)
{
    for(int i=0;i<BUFFER_DIM;i++)
        {
            buf->stato[i]=LIBERO;
        }
}

void produttore(buffer *buf,int id_sem)
{
    Wait_Sem(id_sem,SPAZIO_DISP);   //ATTENDO CHE CI SIA SPAZIO DISPONIBILE
    Wait_Sem(id_sem,MUTEXP);

    int indice=0;
    while((indice<=BUFFER_DIM)&&(buf->stato[indice]!=LIBERO))
        indice++;
    Signal_Sem(id_sem,MUTEXP);       //RILASCIO IL MUTEX


        buf->val[indice]=getpid();
        buf->stato[indice]=PIENO;
    Signal_Sem(id_sem,MSG_DISP);    //SEGNALO UN MSG DISPONIBILE
        printf("PRODUTTORE -->%d -->Prodotto\n",getpid());


 //   exit(1);
}

void consumatore (buffer *  buf,int id_sem)
{
    Wait_Sem(id_sem,MSG_DISP);
    Wait_Sem(id_sem,MUTEXC);


    int indice=0;
    while((indice<=BUFFER_DIM)&&(buf->stato[indice]!=PIENO))
                indice++;

    
    
    Signal_Sem(id_sem,MUTEXC);       //RILASCIO IL MUTEX

    msg mess=buf->val[indice];
    buf->stato[indice]=LIBERO;
    printf("CONSUMATORE PID -->%d -->CONSUMA --> %d\n",getpid(),buf->val[indice]);

    Signal_Sem(id_sem,SPAZIO_DISP);

   // exit(1);
}

void sem_init(int id_sem)
{
    semctl(id_sem,MUTEXC,SETVAL,1);
    semctl(id_sem,MUTEXP,SETVAL,1);
    semctl(id_sem,MSG_DISP,SETVAL,0);
    semctl(id_sem,SPAZIO_DISP,BUFFER_DIM);
}