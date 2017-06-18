#ifndef HEADER_H
#define HEADER_H

#define BUFFER_DIM 	10
#define DISK_DIM 	20
#define N_RICHIESTE 	5
#define N_PROC_UT 	5

#define OK_DEPOSITA 0
#define OK_PRELEVA  1

#include "monitor.h"
#include <sys/types.h>

typedef struct {
	
	unsigned int posizione;
	
	pid_t processo;
}richiesta;

typedef struct{
	
	richiesta buffer[BUFFER_DIM];

	int testa;	
	int coda;
	int liberi;	
}Buffer;

void init_buffer(Buffer *);

void deposita_richiesta(Buffer *,Monitor *,richiesta);
void preleva_richiesta(Buffer *,Monitor *,richiesta *);

void processo_utente(int,int);
void scheduler_disco(int,int);


#endif
