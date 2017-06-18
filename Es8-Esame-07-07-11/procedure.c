/* Procedure.c */

#include "procedure.h"
#include <stdlib.h>


void StackInit (Stack *s,int dim)
{
	s->dati=(Elem*)malloc(sizeof(Elem)*(dim));
	s->dim=dim;
	s->size=0;
	
	//Inizializzazione variabili per la sincronizzazione
	
	pthread_mutex_init(&(s->mutex),NULL);
	pthread_cond_init(&(s->ok_pop),NULL);
	pthread_cond_init(&(s->ok_push),NULL);

}

void StackRemove (Stack *s)
{
	pthread_mutex_destroy(&(s->mutex));	
	pthread_cond_destroy(&(s->ok_pop));
	pthread_cond_destroy(&(s->ok_push));
	free(s->dati);
}

int StackSize (Stack *s)
{
	int size;

	pthread_mutex_lock(&(s->mutex));
		
		size=s->size;
	
	pthread_mutex_unlock(&(s->mutex));
	
	return size;	
}

void StackPush (Stack *s,Elem in)
{
	pthread_mutex_lock(&(s->mutex));
	
	//Il monitor dei PTHREADS e' di tipo signal-and-Continue ==> la condizione deve essere ricontrolalta ogni volta che il processo sospeso viene risvegliato per effetto della signal poiche' potrebbe capitare che la VC sia stata modificata da qualche altra operazione del thread che ha effettuato la signal
	while(s->size==s->dim) //Se lo stack e' pieno non posso inserire
		pthread_cond_wait(&(s->ok_push),&(s->mutex));	//Aspetto una signal da parte del thread che esegue una pop
	//Stack Libero posso inserire

	s->dati[s->size]=in;
	s->size++;
		
	//Segnal all'eventual thread sospeso nell' attesa che nello stack ci fosse un elemento che quell' elemento e' ora disponibile
	pthread_cond_signal(&(s->ok_pop));	

	pthread_mutex_unlock(&(s->mutex));
}

Elem StackPop (Stack *s)
{
	Elem ret;
	
	pthread_mutex_lock(&(s->mutex));
	
	while(s->size==0){
		pthread_cond_wait(&(s->ok_pop),&(s->mutex));
	}
	s->size--;	
	ret=s->dati[s->size];
	
	pthread_cond_signal(&(s->ok_push));	
	
	pthread_mutex_unlock(&(s->mutex));
	
	return ret;
}
