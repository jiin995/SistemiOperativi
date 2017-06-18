#include "header.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc,char *argv[]){

	int id_shm=shmget(IPC_PRIVATE,sizeof(Buffer),IPC_CREAT|0664);
		Buffer *b=(Buffer *)shmat(id_shm,0,0);
		init_buffer(b);

	int id_monitor=shmget(IPC_PRIVATE,sizeof(Monitor),IPC_CREAT|0664);
		Monitor *m=(Monitor *)shmat(id_monitor,0,0);
		init_monitor(m,2);

	int i,res,status;
		
	res=fork();
		if(res==0)
			scheduler_disco(id_shm,id_monitor);

	for(i=0;i<N_PROC_UT;i++){
		res=fork();
			if(res==0)
				processo_utente(id_shm,id_monitor);	
	}

	for(i=0;i<N_PROC_UT+1;i++){
		
		res=wait(&status);
		
		printf("******Processo {%d} terminato con stato %d\n\n",res,status);
	}
		
	
}
