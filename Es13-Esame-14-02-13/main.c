#include "procedure.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define NUM_DOCENTI 1
#define NUM_STUDENTI 10

int main(){
	key_t key_sem,key_shm;
	int id_sem,id_shm;
	int status,pid;	
	
	key_sem=ftok(PATH,ID_SEM);
		id_sem=semget(key_sem,3,IPC_CREAT|0664);
	for(int i=0;i<3;i++){			
		semctl(id_sem,i,SETVAL,1);
		}
	
	key_shm=ftok(PATH,ID);
		id_shm=shmget(key_shm,sizeof(esame_t),IPC_CREAT|0664);
	
	esame_t *esame=shmat(id_shm,0,0);
		esame->prossimo_appello[0]='\0';
		esame->n_prenotati=0;
		esame->n_lettori=0;
	
	int num_processi = NUM_DOCENTI + NUM_STUDENTI;

	for(int i=0; i < num_processi;i++){
		//pid = fork();		
		if((pid=fork()) < 0){
			printf("Errore...\n");
			exit(-1);
		}
		if(pid == 0){
			if(i == 0){
			printf("Generazione processo figlio %d docente con pid %d\n",i,getpid());
			execve("./docente",NULL,NULL);
			}
			else{
			printf("Generazione processo figlio %d studente con pid %d\n",i,getpid());
			execve("./studente",NULL,NULL);
			}
			printf("Qualcosa e' andato storto...\n");
			//sleep(5);
			//exit(0);
		}else{
		//Padre
			//printf("Processo padre attende figli...\n");
		}
	}

	for(int i = 0; i < num_processi;i++){
			wait(&status);	
			if(WIFEXITED(status)){
				printf("Esecuzione terminata normalmente processo %d...\n",i);
				}
			if(WIFSIGNALED(status)){
				printf("Esecuzione terminata di processo %s tramite segnali...\n",(i == 0) ? "docente" : "studente");
				}	
			//printf("Esecuzione terminata incorrettamente processo %s...\n",(i == 0) ? "docente" : "studente");
	}
	shmctl(id_shm,IPC_RMID,0);
	semctl(id_sem,0,IPC_RMID);
	return 0;
	
}

