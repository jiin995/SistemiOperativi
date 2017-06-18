/* Procedure.h */

#ifndef PROCEDURE_H
#define PROCEDURE_H

#define VUOTO 0
#define IN_USO 1
#define PIENO 2
#define BUFFER_DIM 5

#include <pthread.h>

typedef struct {
	int operandi [4];
	int operandi_tot;
} buffer;

typedef struct {
	buffer elaborazione[BUFFER_DIM];
	int stato[BUFFER_DIM];

	int liberi;
	int pieni;	

	pthread_mutex_t m;
	pthread_cond_t ok_elab;	//Su questa var il thread si sospende nel caso il buffer sia vuoto
	pthread_cond_t ok_prod;	//Su questa var il thread si sospende nel caso in cui il buffer sia pieno
}MonitorElaborazioni;

void InitMonitor(MonitorElaborazioni *);
void DestroyMonitor(MonitorElaborazioni *);
void Produci(MonitorElaborazioni *,buffer *);
int  Consuma(MonitorElaborazioni *,buffer *);
void *Richiedente(void *);
void *Elaboratore(void *);

#endif
