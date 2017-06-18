/********PROCEDURE.C*********/
#include "procedure.h"

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void Client(int msg_id_balancer){

	msg_t msg;
		msg.type=getpid();
		msg.PID=getpid();
	int res;

	for(int i=0;i<TOT_MSG;i++){
		res=msgsnd(msg_id_balancer,&msg,(sizeof(msg)-sizeof(long)),0);
			if(res<0){
				printf("[CLIENT] {%d} Errore invio msg\n\n",getpid());
				exit(-1);
			}
		printf("[CLIENT] {%d} Messaggio ==>%d ==>inviato\n\n",getpid(),i);
		
		sleep(1);
	}
	
	printf("[CLIENT] {%d} Ha inviato tutti i messaggi\n\n",getpid());
}

void Server(int msg_id_server){
	msg_t msg;
	
	int res,nmsg;
	int tr=1;	
	nmsg=0;

	while(tr){
		res=msgrcv(msg_id_server,&msg,(sizeof(msg)-sizeof(long)),0,0);
			if(res<0){
				printf("[SERVER] {%d} Errore ricezione msg \n\n",getpid());
				exit(-1); 
			}
		if((msg.PID==-1)&&(msg.type==1))
			tr=0;
		else{
			printf("[SERVER] {%d} Messaggio %d ==>%d \n\n",getpid(),nmsg,msg.PID);
			nmsg++;
		}
	}

	printf("[SERVER] {%d} Ha ricevuto tutti i messaggi n_msg=%d\n\n",getpid(),nmsg);
	
}

void Balancer(int msg_id_balancer,int msg_id_server[TOT_SERVER]){
	msg_t msg;
	
	int res;
	
	for(int i=0;i<(TOT_CLIENT*TOT_MSG);i++){
		res=msgrcv(msg_id_balancer,&msg,(sizeof(msg)-sizeof(long)),0,0);
			if(res<0){
				printf("[BALANCER] {%d} Errore ricezione msg \n\n",getpid());
				exit(-1); 
			}

		printf("[BALANCER] Messaggio ricevuto ==> %d\n\n",msg.PID);

		switch(i%3){
			case 0:
				printf("[BALANCER] Messaggio iniviato al Server 0 ==> %d\n\n",msg.PID);
				res=msgsnd(msg_id_server[0],&msg,(sizeof(msg)-sizeof(long)),0);
			break;

			case 1:
				printf("[BALANCER] Messaggio iniviato al Server 1 ==> %d\n\n",msg.PID);
				res=msgsnd(msg_id_server[1],&msg,(sizeof(msg)-sizeof(long)),0);
			break;
			
			case 2:
				printf("[BALANCER] Messaggio iniviato al Server 2 ==> %d\n\n",msg.PID);
				res=msgsnd(msg_id_server[2],& msg,(sizeof(msg)-sizeof(long)),0);
			break;
		}

		if(res<0){
				printf("[BALANCER] {%d} Errore invio msg \n\n",getpid());
				exit(-1); 
			}
	}

	printf("[BALANCER] Tutti i messaggi smistati\n\n");
	
	msg.type=1;
	msg.PID=-1;
	
	for(int i=0;i<TOT_SERVER;i++){
		res=msgsnd(msg_id_server[i],& msg,(sizeof(msg)-sizeof(long)),0);
			if(res<0){
				printf("[BALANCER] {%d} Errore invio msg \n\n",getpid());
				exit(-1); 
			}
	}
}
