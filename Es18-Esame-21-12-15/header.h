#ifndef HEADER_H
#define HEADER_H

#define BUFFER_DIM 10
#define N_PROD 3
#define N_CONS 4

#include <pthread.h>


typedef struct {
	int indirizzo;
	int dato;
}Buffer;

typedef struct{
	Buffer vettore[BUFFER_DIM];
	
	int testa;
	int coda;
	int n_elem;	

	pthread_mutex_t mutex;
	
	pthread_cond_t ok_produci;
}GestoreIO;

void Inizializza(GestoreIO *);
void Elimina(GestoreIO *);

void Produci(GestoreIO *,Buffer *);
int  Consuma(GestoreIO *,Buffer *);

#endif
