#include "header.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char * argv[]){
	
	key_t Server_key,Printer_key;
		Server_key=ftok(PATH,SERVER_QUEUE);
		Printer_key=ftok(PATH,PRINTER_QUEUE);

	int id_Server_queue,id_Printer_queue;

		id_Server_queue=msgget(Server_key,IPC_CREAT|0664);
			if(id_Server_queue<0){
				perror("MSGGET Fallita");
				exit(-1);
			}

		id_Printer_queue=msgget(Printer_key,IPC_CREAT|0664);
			if(id_Printer_queue<0){
				perror("MSGGET Fallita");
				exit(-1);
			}
	
	int pid=fork();
		if(pid==0){
			Server(id_Server_queue,id_Printer_queue);
		}
	
	pid=fork();
		if(pid==0){
			Printer(id_Printer_queue);
		}

	int i=0;	

	for(i=0;i<N_CLIENT;i++){
	
		pid=fork();
			if(pid==0){
				Client(id_Server_queue);
			}
	}

	for(int i=0;i<(N_CLIENT+2);i++){
		
		wait(NULL);
	}

	msgctl(id_Server_queue,IPC_RMID,0);
	msgctl(id_Printer_queue,IPC_RMID,0);

	return 0;
}
