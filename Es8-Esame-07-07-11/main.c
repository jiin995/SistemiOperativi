// Main.c

#include "procedure.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void *Inserisci (void* s)
{
	Elem el;
	
	Stack *stack=(Stack *) s;	

	for(int i=0;i<N_PUSH;i++){
		el=rand()%11;
		StackPush(stack,el);
		printf("[THREAD_PUSH]: Inserimento %d ==>%d\n",i,el);
		
		sleep(1);
	}
	
	pthread_exit(NULL);
}

void *Preleva (void *s)
{
	Elem v1,v2;
	
	Stack *stack=(Stack *) s;
	
	for(int i=0;i<N_POP;i++){
		v1=StackPop(stack);
		printf("[THREAD_POP]: 1)Prelievo %d <==%d\n",i,v1);		

		v2=StackPop(stack);
		printf("[THREAD_POP]: 2)Prelievo %d <==%d\n",i,v2);	
		
		printf("[THREAD_POP]: %d)Somma   (%d+%d)<=>%d\n",i,v1,v2,v1+v2);	
		
		sleep(3);
	}
	
	pthread_exit(NULL);
}

int main()
{
	pthread_t threads[N_THREAD+1];
	pthread_attr_t attr;
	
	Stack *stack=(Stack *) malloc(sizeof(Stack));
	srand(getpid()+time(NULL));
	StackInit(stack,5);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	for(int i=0;i<N_THREAD;i++){
		printf("Creo Thread %d \n",i);
		pthread_create(&threads[i],&attr,Inserisci,(void *)stack);
	}
	
	printf("Creo Prelievo \n");
	pthread_create(&threads[N_THREAD],&attr,Preleva,(void *) stack);
	
	for(int i=0;i<N_THREAD+1;i++){
		pthread_join(threads[i],NULL);
	}
	
	StackRemove(stack);
}

