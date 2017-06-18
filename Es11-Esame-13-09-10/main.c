#include "monitor.h"
#include "procedure.h"
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main ()
{
	Monitor m;
	int id_scorte,id_liberi,id_magazzino;
	
	id_scorte=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0664);
	id_liberi=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0664);	
	id_magazzino=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0664);
	
	int *liv_scorte;
	int *scaffali_liberi;
	Magazzino *magazzino;
	
	liv_scorte=(int *) shmat(id_scorte,0,0);
		*liv_scorte=0;

	scaffali_liberi=(int *) shmat(id_liberi,0,0);
		*scaffali_liberi=DIM_SCAFFALE;

	magazzino=(Magazzino *) shmat(id_magazzino,0,0);	
		for(int i=0;i<DIM_SCAFFALE;i++)
			(*magazzino)[i].stato=LIBERO;
	
	init_monitor(&m,N_VAR);
	
	int res;
	for(int i=0;i<N_PROC;i++){
		res=fork();
		if(res==0){
			if(i%2==0){
				Cliente(&m,liv_scorte,scaffali_liberi,magazzino);
				exit(0);
			}
			else{
				Fornitore(&m,liv_scorte,scaffali_liberi,magazzino);
				exit(0);
			}
		}
		if(res<0){
			printf("ERRORE NEL CREARE IL PROCESSO \n\n");
			exit(-1);
		}	
	}
	
	for(int i=0;i<N_PROC;i++)
		wait(NULL);
	remove_monitor(&m);
	
	shmctl(id_scorte,IPC_RMID,0);
	shmctl(id_liberi,IPC_RMID,0);
	shmctl(id_magazzino,IPC_RMID,0);

	printf("SHARED MEMORY RIMOSSA \n\n");

}

