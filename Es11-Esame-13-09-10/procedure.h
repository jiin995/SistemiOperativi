#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "monitor.h"

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

#define DIM_SCAFFALE 10
#define N_FORN 5
#define N_CLIE 5
#define N_PROC 10

#define N_VAR 2
#define OK_PROD 0
#define OK_CONS 1

typedef struct{
	unsigned int id_fornitore;
	unsigned int stato;
}scaffale;

typedef scaffale Magazzino[DIM_SCAFFALE];



void Fornitore(Monitor *,int *,int *,Magazzino *);
void Cliente(Monitor *,int *,int *,Magazzino *);

#endif
