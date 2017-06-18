#include "procedure.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void inizializza_prod_cons(PriorityProdCons *p)
{
	p->testa=0;
	p->coda=0;
	p->count=0;

	init_monitor(&(p->m),NVAR);
}

void rimuovi_prod_cons(PriorityProdCons *p)
{
	destroy_monitor(&(p->m));
}

void produci_alta_prio(PriorityProdCons *p)
{
	enter_monitor(&(p->m));
	
	while(p->count==BUFFER_DIM)
		wait_condition(&(p->m),OK_PROD);
	
	int value=rand()%12;
	int old_testa=p->testa;

	p->buffer[p->testa]=value;
	p->testa=(p->testa +1)%BUFFER_DIM;
	p->count++;

	printf("[PRODUTTORE HIGH] {%d} Podotto in pos %d il valore %d\n\n",getpid(),old_testa,value);
	
	signal_condition(&(p->m),OK_CONS);

	leave_monitor(&(p->m));
	
}

void produci_bassa_prio(PriorityProdCons *p)
{
	enter_monitor(&(p->m));
	
	while((p->count==BUFFER_DIM) || (queue_condition(&(p->m),OK_PROD)>0))
		wait_condition(&(p->m),OK_PROD_LOW);
		
	int value=13+(rand()%12);
	int old_testa=p->testa;

	p->buffer[p->testa]=value;
	p->testa=(p->testa+1)%BUFFER_DIM;
	p->count++;
	
	printf("[PRODUTTORE LOW] {%d} Prodotto in pos %d il valore %d\n\n",getpid(),old_testa,value);

	signal_condition(&(p->m),OK_CONS);
	
	leave_monitor(&(p->m));
}

void consuma(PriorityProdCons *p)
{
	enter_monitor(&(p->m));
	
	while(p->count==0)
		wait_condition(&(p->m),OK_CONS);
	
	int value=p->buffer[p->coda];
	int old_coda=p->coda;	

	p->coda=(p->coda+1)%BUFFER_DIM;
	p->count--;
	
	printf("[CONSUMATORE] {%d} Consumato in pos %d il valore %d\n\n",getpid(),old_coda,value);
	
	if(queue_condition(&(p->m),OK_PROD)>0)
		signal_condition(&(p->m),OK_PROD);
	else
		signal_condition(&(p->m),OK_PROD_LOW);
	
	leave_monitor(&(p->m));
}

void produttore_alta_prio(PriorityProdCons *p)
{
	for(int i=0;i<N_HIGH;i++){	
		//printf("[PRODUTTORE HIGH] {%d} Sto per produrre \n\n",getpid());

		produci_alta_prio(p);
		
		sleep(2);
	}
	
	exit(0);
}

void produttore_bassa_prio(PriorityProdCons *p)
{
	for(int i=0;i<N_LOW;i++){
		//printf("[PRODUTTORE LOW] {%d} Sto per produrre \n\n",getpid());
		
		produci_bassa_prio(p);
		
		sleep(1);
	}

	exit(0);
}

void consumatore(PriorityProdCons *p)
{
	for(int i=0;i<N_CONS;i++){
		//printf("[CONSUMATORE] {%d} Sto per consumare \n\n",getpid());		
		
		consuma(p);
		
		sleep(1);
	}
	
	exit(0);
}
