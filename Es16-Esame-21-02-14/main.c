#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void *Capotreno(void *in){
	
	input_thread *m=(input_thread*) in;
	
	printf("[CAPOTRENO] {%d} Instanziato \n\n",m->id_treno);
	
	for(int i=0;i<10;i++){
		scrivi_stazione(m->m,i);
		printf("[CAPOTRENO] {%d} Posizione del treno modificata. La nuova posizione è: %d]\n\n",m->id_treno,i);
		sleep(3);

	}
	
	pthread_exit(NULL);
}

void *Viaggiatore(void *in){
	
	input_thread *m=(input_thread *)in;
	
	printf("[VIAGGIATORE] {%d} Instanziato \n\n",m->id_treno);
	
	for(int i=0;i<3;i++){
		sleep((rand()%6)+1);
		int ris=leggi_stazione(m->m);
		printf("[VIAGGIATORE] Treno n.ro %d Stazione ==>%d\n\n",m->id_treno,ris);
	}
	
	pthread_exit(NULL);
}

int main(){
	
	pthread_attr_t att;
		pthread_attr_init(&(att));
	pthread_attr_setdetachstate(&(att),PTHREAD_CREATE_JOINABLE);

	monitor_treno *m[4];
	for(int i=0;i<4;i++){
		m[i] = ( monitor_treno*) malloc(sizeof(monitor_treno));
	}
	input_thread *in=(input_thread *)malloc(sizeof(input_thread));
	
	for(int i=0;i<4;i++){
		inizializza(m[i]);
	}

	pthread_t capotreno[4];
	pthread_t viaggiatore[10];

	for(int i=0;i<4;i++){
		in->id_treno=i;
		in->m=m[i];
		pthread_create(&(capotreno[i]),&(att),Capotreno,(void *)in);
	}

	int ran;

	srand(time(NULL));
	for(int i=0;i<10;i++){
		ran=rand()%4;
		in->id_treno=ran;
		in->m=m[ran];
		pthread_create(&(viaggiatore[i]),&att,Viaggiatore,(void *)in);
	}
	
	//join dei threads capitreno
	for(int i=0;i<4;i++){
		pthread_join(capotreno[i],NULL);
		printf("Capotreno %d terminato\n",i+1);
	}

	//join dei threads viaggiatori
	for(int i=0;i<10;i++){
		pthread_join(viaggiatore[i],NULL);
		printf("Viaggiatore  %d terminato\n",i+1);
	}
	
	for(int i=0;i<4;i++){
		rimuovi(m[i]);
	}
	
	for(int i=0;i<4;i++){
		free(m[i]);
	}


}
