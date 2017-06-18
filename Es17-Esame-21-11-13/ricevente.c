#include "header.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc,char *argv[]){

	if(argc!=2){
		printf("Errore Richiamo funzione numero parametri non valido :%d\n\n",argc);
		exit(-1);
	}
	
	char char_dest;
		
	char_dest=(char) *argv[1];

	key_t key_mit;
		key_mit=ftok(PATH,char_dest);

	int id_mit;
		id_mit=msgget(key_mit,IPC_CREAT|IPC_EXCL|0664);
		if(id_mit==-1){
			printf("[CHAT RICEVENTE] {%d} Coda ricevente gia' esistente effettuo l'attach \n\n",getpid());
			id_mit=msgget(key_mit,0);
		}
	
	int tr=1;
	int res;	
	
	messaggio_t msg;

	while(tr){
		res=msgrcv(id_mit,&msg,(sizeof(messaggio_t)-sizeof(long)),0,0);
			if(res<0){
				printf("[CHAT RICEVENTE] {%d} Errore ricezione messaggio \n\n",getpid());
				exit(-1);
			}
		if(strcmp(msg.msg,"exit")==0){
			printf("[CHAT RICEVENTE] {%d} Richiesta di chiusura ricevuta \n\n");
			tr=0;
		}
		else{	
			char mess[80];
			strcpy(mess,msg.msg);
			printf("[CHAT RICEVENTE] {%d} Messaggio ricevuto \n**********Mittente :%d, \n**********Messaggio :%s\n\n",getpid(),msg.type,mess);

		}
	
	}
	
	printf("[CHAT RICEVENTE] {%d} Terminazione del processo \n\n");
			
	exit(0);
}
