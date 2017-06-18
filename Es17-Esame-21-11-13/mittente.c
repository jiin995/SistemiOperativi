#include "header.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc,char *argv[]){
	if(argc!=3){
		printf("Errore Richiamo funzione numero parametri non valido :%d\n\n",argc);
		exit(-1);
	}
	
	char char_mi,char_de;
	
	char_mi=(char) *argv[1];
	char_de=(char) *argv[2];
	
	key_t key_mit,key_dest;
	int id_mit,id_des;
				
	key_mit=ftok(PATH,char_mi);
		id_mit=msgget(key_mit,IPC_CREAT|IPC_EXCL|0664);
		if(id_mit==-1){
			printf("[CHAT MITTENTE] {%d} Coda mittente gia' esistente effettuo l'attach \n\n",getpid());
			id_mit=msgget(key_mit,0);
		}
	
	key_dest=ftok(PATH,char_de);
		id_des=msgget(key_dest,IPC_CREAT|IPC_EXCL|0664);
		if(id_des==-1){
			printf("[CHAT MITTENTE] {%d} Coda destinazione gia' esistente effettuo l'attach \n\n",getpid());
			id_des=msgget(key_dest,0);
		}

	
	int tr=1;
	int res;
	char mess[80];
	
	messaggio_t msg;
	
	msg.type=getpid();
	
	while(tr){
		scanf("%s",mess);
		strcpy(msg.msg,mess);
	
		if(strcmp(mess,"exit")==0){
			
			printf("[CHAT MITTENTE] {%d} Ricevuto segnale di terminazione \n\n",getpid());
			tr=0;
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

	printf("[CHAT MITTENTE] {%d} Terminazione del processo \n\n");
	exit(0);
		
}
