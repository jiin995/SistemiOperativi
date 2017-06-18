#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void inizializza(monitor_treno *m){
	
	m->stazione=0;
	m->n_lettori=0;
	m->coda_lettori=0;
	m->coda_scrittori=0;
	m->occupato=0;	
	
	pthread_mutex_init(&(m->m),NULL);
	pthread_cond_init(&(m->ok_read),NULL);
	pthread_cond_init(&(m->ok_write),NULL);
	
}

void rimuovi(monitor_treno *m){
	pthread_mutex_destroy(&(m->m));
	pthread_cond_destroy(&(m->ok_read));
	pthread_cond_destroy(&(m->ok_write));
}

void scrivi_stazione(monitor_treno *m,int stazione){
	
	pthread_mutex_lock(&(m->m)); //Entro nel monitor

	while(m->occupato==1){		//Verifico se attualmente il monitor e' letto o e' in aggiornamento da un altro capotreno
		m->coda_scrittori++;
		pthread_cond_wait(&(m->ok_write),&(m->m));
		m->coda_scrittori--;

	}
	
	m->occupato=1;

	pthread_mutex_unlock(& (m->m)); // lascio il monitor cosi' da dare la possibilita' di altri thread di prendere il mio posto mentre produco
	
	m->stazione=stazione;
	
	pthread_mutex_lock(&(m->m));

	if(m->coda_scrittori>0)
		pthread_cond_signal(&(m->ok_write));
	
	else if(m->coda_lettori>0)
		pthread_cond_signal(&(m->ok_read));

	m->occupato=0;

	pthread_mutex_unlock(&(m->m));
			
	
}

int leggi_stazione(monitor_treno *m){
	
	pthread_mutex_lock(&(m->m));
	
	while((m->occupato==1)&&(m->n_lettori==0)){
		m->coda_lettori++;
		pthread_cond_wait(&(m->ok_read),&(m->m));
		m->coda_lettori--;
	}
	
	m->n_lettori++;

	m->occupato=1;
	
	pthread_cond_signal(&(m->ok_read)); //Risveglio un eventuale lettore sospeso;

	pthread_mutex_unlock(&(m->m));


	int ris=m->stazione;
	

	pthread_mutex_lock(&(m->m));
	
	m->n_lettori--;
	
	if(m->n_lettori==0){
		m->occupato=0;
		pthread_cond_signal(&(m->ok_write));
	}
	
	pthread_mutex_unlock(&(m->m));
	
	return ris;
		
}
