/* Traccia del 13/09/2010 implementata con i semafori */

#include "procedure.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	key_t	key_sem=ftok(FTOK_PATH,FTOK_CHAR_SEM);
	key_t	key_shm=ftok(FTOK_PATH,FTOK_CHAR_SHM);
	
	int id_sem=semget(key_sem,4,IPC_CREAT|0664);
	
	int id_shm=shmget(key_sem,(1*sizeof(int)+(sizeof(scaffale)*MAX_SCORTE)),IPC_CREAT|0664);
	
	int *spazio=(int *) shmat(id_shm,NULL,0);
	scaffale *maga=(scaffale *)(shmat(id_shm,NULL,0)+sizeof(int));
	init_sem(id_sem);
	init_scaffale(maga);
	*spazio=0;
	int pid=0;
	for(int i=0;i<N_PROC;i++)
		{
			pid=fork();
			if(pid==0)	
				{		
					if(i%2==0)
						{	
							produttore(maga,spazio,id_sem);
							exit(1);
						}
					else	
					if(i%2==1)
						{
							cliente(maga,spazio,id_sem);
							exit(1);
						}
				}
		}
	
	for(int i=0;i<N_PROC;i++)
		wait(NULL);

	printf("PADRE TERMINATO CORRETTAMENTE");

	return 0;
}
