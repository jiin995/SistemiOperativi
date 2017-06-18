#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



void *affitta_t (void *in){
	
	Monitor *m=(Monitor *)in;
	int copia;

	for(int i=1;i<N_FILM+1;i++){

		copia=affitta(m,i);

		//printf("[CLIENTE] Ho affittato il film {%d} e ho ottenuto la copia {%d}\n\n",i,copia);
	
		sleep(1);
	
		restituisci(m,i,copia);

		//printf("[CLIENTE] Copia restituita\n\n");
	
	}	
	
	pthread_exit(0);
}

void *stampa_thread(void *in){
	
	Monitor *m=(Monitor *)in;
	
	for(int i=0;i<((N_FILM*4)+2);i++){
		stampa(m);
		sleep(1);
	}
	
	pthread_exit(0);
}



int main(int argc,char *argv[]){
	
	Monitor *m=(Monitor *)malloc(sizeof(Monitor));
		inizializza(m);

	int i=0;
	
	pthread_t threads[5];

	pthread_attr_t att;
		pthread_attr_init(&(att));
		pthread_attr_setdetachstate(&(att),PTHREAD_CREATE_JOINABLE);
	
	int res;	
	

	for(i=0;i<4;i++){
		res=pthread_create(&(threads[i]),NULL,affitta_t,(void *)m);
		if(res<0)
			printf("Errore creazione thread \n\n");
	}
	
	pthread_create(&(threads[i]),&(att),stampa_thread,(void*) m);

	for(i=0;i<5;i++){
		pthread_join(threads[i],NULL);
	}

	distruggi(m);
	
	free(m);

	return 0;
}
