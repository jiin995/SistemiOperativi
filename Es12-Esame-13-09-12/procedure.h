#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "monitor.h"

#define NVAR 3
#define OK_PROD 0
#define OK_PROD_LOW 1
#define OK_CONS 2

#define N_HIGH 3
#define N_LOW 3
#define N_CONS 12 
 
#define N_LOW_PROC 3

#define BUFFER_DIM 3

typedef struct{
	int buffer[BUFFER_DIM];
	
	int testa;
	int coda;
	int count;

	Monitor m;
}PriorityProdCons;

void inizializza_prod_cons(PriorityProdCons *);
void produci_alta_prio(PriorityProdCons *);
void produci_bassa_prio(PriorityProdCons *);
void cosuma(PriorityProdCons *);
void rimuovi_prod_cons(PriorityProdCons *);

void produttore_alta_prio(PriorityProdCons *);
void produttore_bassa_prio(PriorityProdCons *);
void consumatore(PriorityProdCons *);

#endif
