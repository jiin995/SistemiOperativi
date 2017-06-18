#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void Inizializza(GestoreIO * g){
	
	g->testa=0;
	g->coda=0;
	g->n_elem=0;

	pthread_mutex_init(&(g->mutex),NULL);
	
	pthread_cond_init(&(g->ok_produci),NULL);	
	
}

void Elimina(GestoreIO *g){
	
	pthread_mutex_destroy(&(g->mutex));
	
	pthread_cond_destroy(&(g->ok_produci));
}

void Produci(GestoreIO *g,Buffer *b){

	pthread_mutex_lock(&(g->mutex));
	
	
		while(g->n_elem==BUFFER_DIM){
			printf("[PRODUTTORE] {%d} BUFFER PIENO \n\n",getpid());
			pthread_cond_wait(&(g->ok_produci),&(g->mutex));
		}

	int ins=g->testa;
	
		g->testa=(g->testa+1)%BUFFER_DIM;
	
	pthread_mutex_unlock(&(g->mutex));
	
	g->vettore[ins].indirizzo=b->indirizzo;
	g->vettore[ins].dato=b->dato;

	pthread_mutex_lock(&(g->mutex));
		
		g->n_elem=g->n_elem+1;
	
	pthread_mutex_unlock(&(g->mutex));	
		
}

int Consuma(GestoreIO *g,Buffer *b){
	
	pthread_mutex_lock(&(g->mutex));
	
		if(g->n_elem==0){
			pthread_mutex_unlock(&(g->mutex));
			return 1;
		}

		int pre=g->coda;
		
			g->coda=(g->coda+1)%BUFFER_DIM;

	pthread_mutex_unlock(&(g->mutex));


	b->indirizzo=g->vettore[pre].indirizzo;
	b->dato=g->vettore[pre].dato;	

	pthread_mutex_lock(&(g->mutex));
		
		g->n_elem=g->n_elem-1;

		pthread_cond_signal(&(g->ok_produci));
	
	pthread_mutex_unlock(&(g->mutex));

	return 0;	
			
}
