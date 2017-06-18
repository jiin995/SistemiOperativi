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

void azzera_prenotati(int semid,esame_t *esame){
	inizio_lettura(semid,esame);
	accedi_prenotati(semid);
		printf("\n");
		printf("-------Stampa stato esame------\n");
		printf("Data di esame: %s\n",esame->prossimo_appello);
		printf("Numero prenotati: %d\n",esame->n_prenotati);	
		printf("-------------------------------\n");
		printf("\n");
	esame->n_prenotati=0;
	lascia_prenotati(semid);
	fine_lettura(semid,esame);
}

void aggiorna_data(int semid,esame_t* esame,char *data){
	printf("[DOCENTE] Aggiorna data \n\n");
	inizio_scrittura(semid);
		strcpy(esame->prossimo_appello,data);
		printf("[DOCENTE] Appello aggiornato %s",esame->prossimo_appello);
	fine_scrittura(semid);
}	

void esegui(int semid,esame_t* esame){
	char *data;
	for(int i=0;i<3;i++){
		switch(i){
				case 0:
					data=DATA1;
				break;
				case 1:
					data=DATA2;
				break;
				case 2:
					data=DATA3;
				break;
		}
		aggiorna_data(semid,esame,data);
		sleep(3);
		azzera_prenotati(semid,esame);
	}
}

int main(){
	printf("[DOCENTE]{%d} CREATO \n\n",getpid());	

	key_t key_shm,key_sem;
	int id_sem,id_shm;
	esame_t *esame=NULL;
	
	key_sem=ftok(PATH,ID_SEM);
		id_sem=semget(key_sem,0,0);

	key_shm=ftok(PATH,ID);
		id_shm=shmget(key_shm,0,0);

	esame = (esame_t *) shmat(id_shm,0,0);
	
	esegui(id_sem,esame);
	printf("[STUDENTE]{%d} TERMINATO \n\n",getpid());
	return 0;
}
