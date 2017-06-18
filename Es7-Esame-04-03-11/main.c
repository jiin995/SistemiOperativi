#include "procedure.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>

int inbox_Send,inbox_Recive;

int main()
{

	key_t inbox_Send_Key,inbox_Recive_Key;
	inbox_Send_Key=ftok(PATH,INBOX_SEND);
		inbox_Send=msgget(inbox_Send_Key,IPC_CREAT|0660);
			if(inbox_Send==-1)
				return -1;
	inbox_Recive_Key=ftok(PATH,INBOX_RECIVE);
		inbox_Recive=msgget(inbox_Recive_Key,IPC_CREAT|0660);
			if(inbox_Recive==-1)
				return -1;
	int r=fork();
	int client_pid[3];
	if(r==0)
		Server_Main(inbox_Recive,inbox_Send);
	else if (r>0){
		for(int i=0;i<N_CLIENT;i++){
			printf("Creo Client %d\n",i);
			client_pid[i]=fork();
			if(client_pid[i]==0){
				Client(inbox_Send,inbox_Recive);
				return 0;
			}
			else if(client_pid[0]<0)
				return -1;
		}
			for(int i=0;i<N_CLIENT;i++)
               			 wait(NULL);
       			msgs send;
                		send.type=getpid();
                		send.n1=-1;
                		send.n2=-1;
        		msgsnd(inbox_Send,&send,sizeof(msgs)-sizeof(long),IPC_NOWAIT);
        			wait(NULL);
        		printf("Server Terminato");
			msgctl(inbox_Send,0,IPC_RMID);
			msgctl(inbox_Recive,0,IPC_RMID);
		}
	else if(r<0)
		return -1;
	/*int count=0;
	while(count <3){
		wait(&status);	
	}*/
}
