#ifndef PROCEDURE_H
#define PROCEDURE_H

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2

#define N_THREAD 5
#define N_VOLI 3

#include <pthread.h>

typedef struct{
	int identificativo;
	int quota;
}Volo;

typedef struct {
	Volo vett_voli[N_VOLI];
	int stato_voli[N_VOLI];

	int in_volo;	
	
	pthread_mutex_t m;
	
	pthread_cond_t ok_inser;
}GestioneVoli;

typedef struct  {
	GestioneVoli *gest;
	int id;
}param;

void InserisciVolo(GestioneVoli *,int );
void RimuoviVolo(GestioneVoli *,int );
void AggiungiVolo(GestioneVoli *,int,int);
void PrintVolo(Volo);

void InitMonitor(GestioneVoli *);
void DestroyMonitor(GestioneVoli *);

void *ControlloreVolo(void*);


#endif
