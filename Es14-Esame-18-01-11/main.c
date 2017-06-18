#include "procedure.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	int msg_id_balancer=msgget(IPC_PRIVATE,IPC_CREAT|0664);
		if(msg_id_balancer < 0) {
			perror("Errore msgget() coda balancer");
			exit(1);
		}

	int msg_id_server[TOT_SERVER];

	for(int i=0;i<TOT_SERVER;i++){
		msg_id_server[i]=msgget(IPC_PRIVATE,IPC_CREAT|0664);
			if(msg_id_server[i] < 0) {
				perror("Errore msgget() coda server");
				exit(1);
			}
	}

	int res=fork();
	
	if(res==0){
		printf("[PADRE] BALANCER CREATO {%d} \n\n",getpid());
		Balancer(msg_id_balancer,msg_id_server);
		exit(0);
	}
	
	for(int i=0;i<TOT_SERVER;i++){
		res=fork();
		if(res==0){
			printf("[PADRE] SERVER 5d CREATO {%d} \n\n",i,getpid());
			Server(msg_id_server[i]);
			exit(0);
		}

		if(res<0) {
			perror("Errore fork() client");
			exit(1);
		}
	}
	
	for(int i=0;i<TOT_CLIENT;i++){	
		res=fork();
		if(res==0){
			printf("[PADRE] Client CREATO {%d} \n\n",getpid());
			Client(msg_id_balancer);
			exit(0);
		}

		if(res<0) {
			perror("Errore fork() Server ");
			exit(1);
		}
	}
	
	int status;	
	
	for(int i=0;i<(TOT_CLIENT+TOT_SERVER+1);i++){
		res=wait(&status);
		if(res<0){
			perror("ERRORE WAIT ");
			exit(-1);
		}
		
		printf("Il processo %d è terminato correttamente %d\n\n", res,i);
		
	}

	msgctl(msg_id_balancer,IPC_RMID,NULL);
	for(int i=0; i<TOT_SERVER;i++)
		msgctl(msg_id_server[i],IPC_RMID,NULL);
		
}
