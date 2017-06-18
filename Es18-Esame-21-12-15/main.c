#include "header.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

void * Produttore (void *in){
	
	GestoreIO *g=(GestoreIO *)in;
	

	
	Buffer *b=(Buffer *)malloc(sizeof(Buffer));	
	b->indirizzo=rand()%10;	
	b->dato=rand()%10;
	
	for(int i=0;i<N_PROD;i++){
		printf("[PRODUTTORE] {%d} \nIndirizzo: %d, \n Dato: %d \n\n",pthread_self(),b->indirizzo,b->dato);	
		Produci (g,b);
	
		b->indirizzo++;
		b->dato++;
		
		sleep(2);
		
	}

	pthread_exit(0);
}

void *Consumatore(void *in){
	GestoreIO *g=(GestoreIO *)in;

	Buffer *b=(Buffer *)malloc(sizeof(Buffer));

	printf("[Consumatore] {%d} \n\n ",pthread_self());

	for(int i=0;i<N_CONS;i++){

		while(Consuma(g,b)==0){

			printf("[CONSUMATORE] {%d} \nIndirizzo: %d, \n Dato: %d\n\n",pthread_self(),b->indirizzo,b->dato);			
	
		}
		sleep(1);	
	}

	pthread_exit(0);
}

int main(int argc,char *argv[]){
	
	GestoreIO *g=(GestoreIO *)malloc(sizeof(GestoreIO));
		Inizializza(g);

	pthread_attr_t att;
	
	pthread_attr_init(&(att));
	
	pthread_attr_setdetachstate(&(att),PTHREAD_CREATE_JOINABLE);

	pthread_t Produttori[4];
	pthread_t Consumatori[2];
	
	for(int i=0;i<4;i++){
		pthread_create(&(Produttori[i]),&att,Produttore,(void *)g);
	}
	
	for(int i=0;i<2;i++){
		pthread_create(&(Consumatori[i]),&att,Consumatore,(void *)g);
	}

	for(int i=0;i<4;i++){
		pthread_join((Produttori[i]),NULL);
	}
	
	for(int i=0;i<2;i++){
		pthread_join((Consumatori[i]),NULL);
	}
	
	Elimina(g);

}
