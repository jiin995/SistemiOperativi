#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void inizializza(Monitor *m){
	
	int i=0;
	
	pthread_mutex_init(&(m->mutex),NULL);
	
	for(i=0;i<N_FILM;i++){	
		pthread_cond_init(&(m->ok_affitta[i]),0);
	}	
	
	for(i=0;i<(N_FILM*2);i++){
		m->dvd[i].identificativo_film=1+(i%3);
		m->dvd[i].identificativo_copia=1+(i%2);
		m->dvd[i].stato=DISPONIBILE;	
	}

	for(i=0;i<N_FILM;i++){
		m->copie_disp[i]=2;
	}
}

void distruggi(Monitor *m){

	int i=0;
	
	pthread_mutex_destroy(&(m->mutex));
	
	for(i=0;i<N_FILM;i++){	
		pthread_cond_destroy(&(m->ok_affitta[i]));
	}	
}

int affitta(Monitor *m,int id_film){

	int i=0;
	
	if((id_film>3)&&(id_film<=0))
		return -1;	

	int id=id_film-1;
	
	pthread_mutex_lock(&(m->mutex));
	
	//printf("RICHIEDO COPIA FILM %d, %d \n\n",id_film,id);

		while(m->copie_disp[id]==0){
			pthread_cond_wait(&(m->ok_affitta[id]),&(m->mutex));
		}
	
	//printf("[AFFITTA] Copia disponibile procedo al prelievo \n\n");
	m->copie_disp[id]--;	
	
	if(m->dvd[id].stato==DISPONIBILE)
		m->dvd[id].stato=AFFITTATO;
	
	else{
		id=id+3;
	
		if(m->dvd[id].stato==DISPONIBILE)
			m->dvd[id].stato=AFFITTATO;
	}	
	
	pthread_mutex_unlock(&(m->mutex));

	return m->dvd[id].identificativo_copia;
}

void restituisci(Monitor *m,int id_film,int id_copia){
	
	int i=0;
	
	pthread_mutex_lock(&(m->mutex));
	
	int id=id_film-1;

	//printf("RESTITUISCO COPIA FILM %d, %d , %d\n\n",id_film,id_copia,id);

		m->copie_disp[id]++;

		if(m->dvd[id].identificativo_copia==id_copia)
			m->dvd[id].stato=DISPONIBILE;
		else{
			id+=3;
			m->dvd[id].stato=DISPONIBILE;
		}
		
		if(id>2)
			id-=3;

		pthread_cond_signal(&(m->ok_affitta[id]));

	pthread_mutex_unlock(&(m->mutex));

}

void stampa(Monitor *m){
	
	int i;

	printf("********************************\n\n");
		
	for(i=0;i<(N_FILM*2);i++){

		printf("*****[FILM]  :{%d} \n*****[COPIA] :{%d} \n*****[STATO] :{%d} \n\n",
					m->dvd[i].identificativo_film,
					m->dvd[i].identificativo_copia,
					m->dvd[i].stato);

	}
		
	for(i=0;i<N_FILM;i++){
		printf("*****[FILM]  :{%d} \n*****[COPIE] :{%d} \n\n",i+1,m->copie_disp[i]);
	}

	printf("********************************\n\n");
}
