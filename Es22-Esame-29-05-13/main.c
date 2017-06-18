#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

int main(int argc,char *argv[]){
	
	char *args[1];
	

	key_t key_richieste=ftok(FTOK_PATH,FTOK_CHAR_RIC);
		int id_richieste=msgget(key_richieste,IPC_CREAT|IPC_EXCL|0664);
			if(id_richieste<0)
				id_richieste=msgget(key_richieste,0664);
	
	key_t key_risposte=ftok(FTOK_PATH,FTOK_CHAR_RIS);
		int id_risposte=msgget(key_risposte,IPC_CREAT|IPC_EXCL|0664);
			if(id_risposte<0)
				id_risposte=msgget(key_risposte,0664);
	int i;
	int res=fork();
		if(res==0){	
			res=execve("./server",NULL,NULL);
				if(res<0){
					printf("Errore nell'avviare il server \n\n");
					exit(-1);
				}
		}	
	
	for(i=0;i<5;i++){
	
		res=fork();
		if(res==0){	
			res=execve("./client",NULL,NULL);
				if(res<0){
					printf("Errore nell'avviare il client %d \n\n",i);
					exit(-1);
				}
		}

	}
	
	for(i=0;i<5;i++){
		res=wait(NULL);
			printf("[PROCESSO TERMINATO] {%d}\n\n",res);
	}

	msg_richiesta *msg_r=(msg_richiesta *)malloc(sizeof(msg_richiesta));
		msg_r->type=getpid();
		msg_r->v1=-1;
		msg_r->v2=-1;

		res=msgsnd(id_richieste,msg_r,sizeof(msg_richiesta)-sizeof(long),0);
			if(res<0){
				printf("[MAIN] {%d} Errore nell'invio del messaggio \n\n");
				exit(-1);
			}

	res=wait(NULL);
			printf("[PROCESSO TERMINATO] {%d}\n\n",res);
		

	msgctl(id_richieste,IPC_RMID,0);
	msgctl(id_risposte,IPC_RMID,0);

}

