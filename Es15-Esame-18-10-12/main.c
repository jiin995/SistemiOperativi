#include "header.h"

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	key_t op_key=ftok(FTOK_PATH,FTOK_CHAR_Q_OP_1);
		int id_op=msgget(op_key,IPC_CREAT|0664);
			if(id_op<0){
				perror("ERRORE msgget");
				exit(-1);
			}
	
	key_t ris_key=ftok(FTOK_PATH,FTOK_CHAR_Q_RIS_1);
		int id_ris=msgget(ris_key,IPC_CREAT|0664);
			if(id_ris<0){
				perror("ERRORE msgget");
				exit(-1);
			}
	
	int res=fork();
		if(res==0){
			execve("p1",0,0);
		}
	res=fork();
		if(res==0){
			execve("p2",0,0);
		}
	res=fork();
		if(res==0){
			execve("p3",0,0);
		}
	res=fork();
		if(res==0){
			execve("p4",0,0);
		}
	res=fork();
		if(res==0){
			execve("p5",0,0);
		}
	res=fork();
		if(res==0){
			execve("p6",0,0);
		}
	
	for(int i=0;i<6;i++){
		res=wait(NULL);
		printf("{%d}Processo Terminato\n",res);
	}
	
	return 0;
}
