/*********main.c************/

#include "procedure.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main ()
{
	
	int res=0;
	
	int id_shm=shmget(IPC_PRIVATE,sizeof(PriorityProdCons),IPC_CREAT|0664);
	
	PriorityProdCons *p=(PriorityProdCons *)shmat(id_shm,0,0);
	
	inizializza_prod_cons(p);
	
	srand(time(NULL));
	
	res=fork();
		if(res==0){
			consumatore(p);
		}
	
	res=fork();
		if(res==0){
			produttore_alta_prio(p);
		}
	
	for(int i=0;i<N_LOW_PROC;i++){
		res=fork();
		
		if(res==0){
			produttore_bassa_prio(p);
		}
	}
	
	for(int i=0;i<(N_LOW_PROC+2);i++){
		wait(NULL);
	}

	printf("TUTTI I PROCESSI SONO TERMINATI \n\n");

	rimuovi_prod_cons(p);
	shmctl(id_shm,IPC_RMID,0);
	
	return 0;
}

