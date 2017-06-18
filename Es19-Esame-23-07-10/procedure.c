#include "header.h"

#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int Preleva(int id_queue,message_t *msg){
	
	int res=0;	
		res=msgrcv(id_queue,msg,(sizeof(message_t)-sizeof(long)),0,0);
	return res;
}

int Inserisci(int id_queue,message_t *msg){

	int res=0;
		res=msgsnd(id_queue,msg,(sizeof(message_t)-sizeof(long)),0);
	return res;
}

void Client(int Server_queue){
	
	message_t *msg=(message_t *) malloc(sizeof(message_t));
	
	msg->type=getpid();
	msg->pid=getpid();
	
	int res=0;
	
	for(int i=0;i<N_RICHIESTE;i++){
	
		res=Inserisci(Server_queue,msg);
			if(res<0){
				printf("[CLIENT] {%d} Errore nell'invio del messaggio\n\n",getpid());
				exit(-1);
			}

		printf("[CLIENT] {%d} Messaggio inviato \n\n",getpid());

		sleep(1);	
	}

	msg->pid=-1;

	res=Inserisci(Server_queue,msg);
			if(res<0){
				printf("[CLIENT] {%d} Errore nell'invio del messaggio\n\n",getpid());
				exit(-1);
			}
	
	printf("[CLIENT] {%d} Processo Terminato \n\n",getpid());
	
	exit(0);
}	

void Server(int Server_queue,int Printer_queue){

	message_t Buffer[N_BUF];
	message_t *msg=(message_t *) malloc(sizeof(message_t));

	int res=0;
	int count=0;
	int count_end=0;
	
	while(1){
		
		res=Preleva(Server_queue,msg);
			if(res<0){
				printf("[SERVER] {%d} Errore ricezione messaggio\n\n",getpid());
				exit(-1);			
			}
		
		printf("[SERVER] {%d} Messaggio ricevuto :%d\n\n",getpid(),msg->pid);
		
		if(msg->pid==-1){
			
			if(count_end==5){	
				printf("[SERVER] {%d} Processo Terminato :%d \n\n",getpid(),msg->pid);	
				Inserisci(Printer_queue,msg);
				exit(0);		
			}
		}

		else{
			Buffer[count].type=msg->type;
			Buffer[count].pid=msg->pid;
			count++;
			
			if(count==N_BUF){
				while((count!=0)){
					res=Inserisci(Printer_queue,&(Buffer[count-1]));
					count--;
				}
			}
		}
	}
}

void Printer(int id_queue){

	int res=0;
	int count=0;	

	message_t * msg=(message_t *) malloc(sizeof(message_t));
		
	while(1){
	
		res=Preleva(id_queue,msg);
			if(res<0){
				printf("[PRINTER] {%d} Errore ricezione messaggio",getpid());
				exit(-1);
			}
		
		if(msg->pid==-1){
				printf("[PRINTER] {%d} Processo Terminato\n\n",getpid());
				exit(0);
		}
		
		count++;			
	
		printf("******[PRINTER] {%d} Messaggio %d ricevuto ==> %d \n",getpid(),count,msg->pid);
	}
}
