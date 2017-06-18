#ifndef PROCEDURE_H
#define PROCEDURE_H

#define DISPONIBILE 0
#define AFFITTATO 1

#define N_FILM 3


#include <pthread.h>

typedef struct{
	int identificativo_film;
	int identificativo_copia;
	int stato;
}DVD;

typedef struct {
	DVD dvd[N_FILM*2];
	
	int copie_disp[N_FILM];	

	pthread_mutex_t mutex;
	
	pthread_cond_t ok_affitta[N_FILM];
}Monitor;

void inizializza(Monitor *);
void distruggi(Monitor *);

int affitta(Monitor *,int);
void restituisci(Monitor *,int ,int);
void stampa(Monitor *);

#endif
