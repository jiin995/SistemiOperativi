#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <pthread.h>

typedef struct{

	int stazione;

	pthread_mutex_t m;
	
	int n_lettori;
	int occupato;

	int coda_scrittori;
	int coda_lettori;
	
	pthread_cond_t ok_read;
	pthread_cond_t ok_write;

}monitor_treno;

typedef struct{

	int id_treno;
	monitor_treno *m;

}input_thread;	

void inizializza (monitor_treno *);
void rimuovi (monitor_treno *);
int leggi_stazione(monitor_treno *);
void scrivi_stazione(monitor_treno *,int stazione);

#endif
