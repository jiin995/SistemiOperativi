
#include "header.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

void mittente (int id_mit,int id_des){

	
		

	int res;
	char mess[80];
	
	messaggio_t msg;
	
	msg.type=getpid();
	
	while(1){
		scanf("%s",mess);
		strcpy(msg.msg,mess);
	
		if(strcmp(mess,"exit")==0){
			
			printf("[CHAT MITTENTE] {%d} Ricevuto segnale di terminazione \n\n",getpid());
			res=msgsnd(id_des,&msg,(sizeof(messaggio_t)-sizeof(long)),0);
			if(res<0){
				printf("[MITTENTE] {%d}Errore nell'esecuzione della send",getpid());
				exit(-1);
			}
		}
		else{
			res=msgsnd(id_mit,&msg,(sizeof(messaggio_t)-sizeof(long)),0);
			if(res<0){
				printf("[MITTENTE] {%d}Errore nell'esecuzione della send",getpid());
				exit(-1);
			}
		}
				
	}
		
}

void ricevente(int id_mit){
	
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
