#include "procedure.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{

    key_t shm_key=ftok(FTOK_PATH,FTOK_SHM_CHAR);
    key_t sem_key=ftok(FTOK_PATH,FTOK_SEM_CHAR);

    int id_shm=shmget(shm_key,sizeof(buffer),IPC_CREAT|0664);
    int id_sem=semget(sem_key,NSEM,IPC_CREAT|0664);

    buffer *buf=(buffer *) shmat(id_shm,0,0);

    semctl(id_sem,MUTEXC,SETVAL,1);
    semctl(id_sem,MUTEXP,SETVAL,1);
    semctl(id_sem,MSG_DISP,SETVAL,0);
    semctl(id_sem,SPAZIO_DISP,SETVAL,5);
    buffer_init(buf);
    
    int pid;
    for(int i=0;i<10;i++)
        {
           pid=fork();
            if(pid==0)
                if(i%2==0)
                    {
                        produttore(buf,id_sem);
                        exit(1);
                    }      
            else
                if(i%2==1)
                    {
                        consumatore(buf,id_sem);
                        exit(1);
                    }
        }
    for(int i=0;i<10;i++)
        wait(NULL);
    shmctl(id_shm,IPC_RMID,0);
	semctl(id_sem,0,IPC_RMID);
    return 0;
}
