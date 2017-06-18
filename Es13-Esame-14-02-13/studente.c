#include "procedure.h"

#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

void prenota(int semid,esame_t * esame){
	accedi_prenotati(semid);
		esame->n_prenotati +=1;
	lascia_prenotati(semid);
}

void stampa_appello(int semid,esame_t *esame){
	inizio_lettura(semid,esame);
		printf("[STUDENTE] {%d} Esame prenotato ==> Data Appello: %s\n",getpid(),esame->prossimo_appello);
	fine_lettura(semid,esame);
}

void esegui(int semid,esame_t* esame){
	int t;
	t=rand()%8;
	printf("Sono studente  %d e aspetto tempo %d secondi\n",getpid(),t);
	sleep(t);
	stampa_appello(semid,esame);
	prenota(semid,esame);
}

int main(){
	
	printf("[STUDENTE]{%d} CREATO \n\n",getpid());
	key_t key_shm,key_sem;
	int id_sem,id_shm;
	
	esame_t * esame=0;	

	key_shm=ftok(PATH,ID);
		id_shm=shmget(key_shm,0,0);
		esame=(esame_t*)shmat(id_shm,0,0);		

	key_sem=ftok(PATH,ID_SEM);
		id_sem=semget(key_sem,0,0);
	
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);
	
	esegui(id_sem,esame);

	printf("[STUDENTE]{%d} TERMINATO \n\n",getpid());
	return 0;
}
