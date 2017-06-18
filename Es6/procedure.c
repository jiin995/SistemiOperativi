#include "procedure.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

int Wait_Sem(int id_sem,int n_sem){
	struct sembuf sbuf;
		sbuf.sem_num=n_sem;
		sbuf.sem_op=-1;
		sbuf.sem_flg=0;
	return semop(id_sem,&sbuf,1);
}

int Signal_Sem(int id_sem,int n_sem){
	struct sembuf sbuf;
		sbuf.sem_num=n_sem;
		sbuf.sem_op=1;
		sbuf.sem_flg=0;
	return semop(id_sem,&sbuf,1);
}

int init_sem(int id_sem)
{
	semctl(id_sem,MUTEX,SETVAL,1);
	semctl(id_sem,MUTEX_LIV_SCORTE,SETVAL,1);
	semctl(id_sem,SOSPENSIONE_P,SETVAL,0);
	semctl(id_sem,SOSPENSIONE_C,SETVAL,0);
}

int produci(scaffale * sc,int lib){
	sc[lib].id_fornitore=getpid();
	(sc+lib)->stato=OCCUPATO;
	sleep(2);
}

int consuma(scaffale *sc,int pieno){
	sc[pieno].id_fornitore=0;
	sc[pieno].stato=LIBERO;
	sleep(2);
}

void init_scaffale(scaffale *sc){
	for(int i=0;i<MAX_SCORTE;i++)
		sc[i].stato=LIBERO;
}

void produttore(scaffale *sc,int *liv_scorte, int id_sem){
int tr=1;
int lib=0;
	printf("PRODUTTORE --> %d --> CREATO \n",getpid());
	for (int nrip=0;nrip<N_RIP;nrip++)
		{
			while(tr)
			{
				Wait_Sem(id_sem,MUTEX_LIV_SCORTE);
				if(*liv_scorte>=100){			
					Signal_Sem(id_sem,MUTEX_LIV_SCORTE);			
					Wait_Sem(id_sem,SOSPENSIONE_P);
				}
				else
					{
						*liv_scorte+=1;
						printf("PRODUTTORE --> %d -->  %d\n",getpid(),*liv_scorte);
						Signal_Sem(id_sem,MUTEX_LIV_SCORTE);			
						tr=0;
					}
			}
			
			tr=1;
			lib=0;
			int i=0;

			Wait_Sem(id_sem,MUTEX);
				while((lib==0)&&(i<MAX_SCORTE))
					{
						if((sc+i)->stato==LIBERO)
							{
								sc[i].stato==IN_USO;
								lib=i;
							}
						i++;
					}
			Signal_Sem(id_sem,MUTEX);
			produci(sc,lib);		
			printf("PRODUTTORE --> %d --> PRODOTTO \n",getpid());
			Signal_Sem(id_sem,SOSPENSIONE_C);	
		}
}

void cliente (scaffale * sc,int *liv_scorte,int id_sem)
{
	int tr=1;
	int lib=0;
	int i=0;
	
	printf("CONSUMATORE --> %d --> CREATO \n",getpid());
	for(int nrip=0;nrip<N_RIP;nrip++)
		{
			while(tr)
				{
					Wait_Sem(id_sem,MUTEX_LIV_SCORTE);
					if(*liv_scorte==0)
						{
							Signal_Sem(id_sem,MUTEX_LIV_SCORTE);	
							Wait_Sem(id_sem,SOSPENSIONE_C);
						}
					else
						{
							*liv_scorte-=1;
							printf("PRODUTTORE --> %d -->  %d\n",getpid(),*liv_scorte);
							Signal_Sem(id_sem,MUTEX_LIV_SCORTE);
							tr=0;
						}
				}
		
			tr=1;
			lib=0;
			int i=0;
			Wait_Sem(id_sem,MUTEX);
				while((lib==0)&&(i<MAX_SCORTE))
					{
						if((sc+i)->stato==OCCUPATO)
							{
								sc[i].stato==IN_USO;
								lib=i;
							}
						i++;
					}
			Signal_Sem(id_sem,MUTEX);
			printf("CONSUMATORE --> %d --> CONSUMATO --> %d \n",getpid(),(sc+lib)->id_fornitore);
			consuma(sc,lib);		
			Signal_Sem(id_sem,SOSPENSIONE_P);	
		}
		
}


	











































