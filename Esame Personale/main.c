	#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define N_FOR 2
#define N_CLI 3

int main(int argc,char *argv[]){

	int res;
	
	int id_richieste=msgget(IPC_PRIVATE,IPC_CREAT|0664);
		if(id_richieste<0){
			printf("Erorre get coda \n\n");
			exit(-1);
		}

	int id_risposte=msgget(IPC_PRIVATE,IPC_CREAT|0664);
		if(id_risposte<0){
			printf("Erorre get coda \n\n");
			exit(-1);
		}

	res=fork();
		if(res==0){
			gestore(id_richieste,id_risposte);
			exit(0);
		}
	
	int i;
	
	for(i=0;i<N_FOR;i++){
		res=fork();
			if(res==0){
				fornitore(id_richieste);
				exit(0);
			}	
	}

	for(i=0;i<N_CLI;i++){
		res=fork();
			if(res==0){
				cliente(id_richieste,id_risposte);
				exit(0);
			}
	}

	for(i=0;i<N_CLI+N_FOR+1;i++){

		res=wait(NULL);
		printf("Processo {%d} Terminato \n\n",res);
	}

	msgctl(id_risposte,IPC_RMID,0);
	msgctl(id_richieste,IPC_RMID,0);
}
