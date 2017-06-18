#include "header.h"

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define N_RICHIESTE 5

int main(){
	
	key_t key_richieste=ftok(FTOK_PATH,FTOK_CHAR_RIC);
		int id_richieste=msgget(key_richieste,IPC_CREAT|IPC_EXCL|0664);
			if(id_richieste<0)
				id_richieste=msgget(key_richieste,0664);
	
	key_t key_risposte=ftok(FTOK_PATH,FTOK_CHAR_RIS);
		int id_risposte=msgget(key_risposte,IPC_CREAT|IPC_EXCL|0664);
			if(id_risposte<0)
				id_risposte=msgget(key_risposte,0664);

	printf("[CLIENT] {%d} avviato \n\n",getpid());
	
	srand(time(NULL));

	int i;

	msg_richiesta *msg_r=(msg_richiesta *)malloc(sizeof(msg_richiesta));
	msg_risposta *risposta=(msg_risposta *)malloc(sizeof(msg_risposta));

	msg_r->type=getpid();
	
	int res;

	for(i=0;i<N_RICHIESTE;i++){
	
		msg_r->v1=rand()%11;
		msg_r->v2=rand()%11;

		res=msgsnd(id_richieste,msg_r,sizeof(msg_richiesta)-sizeof(long),0);
			if(res<0){
				printf("[CLIENT] {%d} Errore nell'invio del messaggio \n\n");
				exit(-1);
			}

		printf("[CLIENT] {%d} Messaggio inviato v1=>%d v2=>%d \n\n",msg_r->type,msg_r->v1,msg_r->v2);
		
		res=msgrcv(id_risposte,risposta,sizeof(msg_risposta)-sizeof(long),msg_r->type,0);
			if(res<0){
				printf("[CLIENT] {%d} Errore nella ricezione del messaggio \n\n");
				exit(-1);
			}

		printf("[CLIENT] {%d} Messaggio Ricevuto v1*v2 --> %d*%d=%d \n\n",msg_r->type,msg_r->v1,msg_r->v2,risposta->ris);
		

	}
	
	
	exit(0);
	
}
