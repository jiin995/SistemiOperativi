#include "procedure.h"

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void Wait_Sem(int idsem,int nsem){
	struct sembuf sem_buf;
		sem_buf.sem_num=nsem;
		sem_buf.sem_flg=0;
		sem_buf.sem_op=-1;
	semop(idsem,&sem_buf,1);	
}

void Signal_Sem(int idsem,int nsem){
	struct sembuf sem_buf;
		sem_buf.sem_num=nsem;
		sem_buf.sem_flg=0;
		sem_buf.sem_op=1;
	semop(idsem,&sem_buf,1);
}

void inizio_lettura(int idsem,esame_t *esame){
	Wait_Sem(idsem,MUTEX);
		esame->n_lettori++;
	if(esame->n_lettori==1)
		Wait_Sem(idsem,APPELLO); // se sono il primo lettore allora attendo l'accesso in mutua esclusione per i lettori
	Signal_Sem(idsem,MUTEX);
}

void fine_lettura(int idsem,esame_t *esame){
	Wait_Sem(idsem,MUTEX);
		esame->n_lettori--;
	if(esame->n_lettori==0)	//Se sono l'ultimo lettore allora lascio l'accesso ai scrittori
		Signal_Sem(idsem,APPELLO);
	Signal_Sem(idsem,MUTEX);
}

void inizio_scrittura(int idsem){
	Wait_Sem(idsem,APPELLO);
}

void fine_scrittura(int idsem){
	Signal_Sem(idsem,APPELLO);
}

void accedi_prenotati(int idsem){
	Wait_Sem(idsem,PRENOTATI);
}

void lascia_prenotati(int idsem){
	Signal_Sem(idsem,PRENOTATI);
}
