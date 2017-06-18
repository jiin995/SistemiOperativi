#include "header.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void init_buffer(Buffer *b){

	b->testa=0;
	b->coda=0;

	b->liberi=BUFFER_DIM;
}

void deposita_richiesta(Buffer *b,Monitor *m,richiesta r){
	
	enter_monitor(m);
	
		if(b->liberi==0){
			wait_condition(m,OK_DEPOSITA);
		}
	
	b->buffer[b->testa].posizione=r.posizione;
	b->buffer[b->testa].processo=r.processo;

	b->testa=(b->testa +1)%BUFFER_DIM;
	
	b->liberi--;	
	signal_condition(m,OK_PRELEVA);

	leave_monitor(m);
	
}

void preleva_richiesta(Buffer *b,Monitor *m,richiesta *r){

	enter_monitor(m);
	
		if(b->liberi==BUFFER_DIM){
			wait_condition(m,OK_PRELEVA);
		}

	r->processo=b->buffer[b->coda].processo;
	r->posizione=b->buffer[b->coda].posizione;
	
	b->coda=(b->coda+1)%BUFFER_DIM;

	b->liberi++;
	signal_condition(m,OK_DEPOSITA);
	
	leave_monitor(m);
}

void processo_utente(int id_shm,int id_monitor){

	Buffer *b=(Buffer *) shmat(id_shm,0,0);
	
	Monitor *m=(Monitor *)shmat(id_monitor,0,0);
	
	richiesta r;
	
	srand(time(NULL));
		
	r.processo=getpid();

	for(int i=0;i<N_RICHIESTE;i++){

		r.posizione=rand()%20;
		
		printf("[PROCESSO] {%d} Richiesta inviata \n+++Posizione -->%d\n+++Valore -->%d\n\n",getpid(),r.posizione,r.processo);
	
		deposita_richiesta(b,m,r);

	}

	exit(0);
}

void scheduler_disco(int id_shm,int id_monitor){
	
	Buffer *b=(Buffer *)shmat(id_shm,0,0);

	Monitor *m=(Monitor *)shmat(id_monitor,0,0);
	
	richiesta *r=(richiesta *)malloc(sizeof(richiesta));
		
	int disco[DISK_DIM];

	int old_pos=0;
	int sleep_time=0;

	for(int i=0;i<(N_PROC_UT*N_RICHIESTE);i++){

		preleva_richiesta(b,m,r);

		if((r->posizione<0)||(r->posizione>20)){
				printf("[SCHEDULER DISCO] Posizione Errata \n\n");
				exit(-1);
		}

		printf("[SCHEDULER DISCO] {%d} Richiesta ricevuta \n---Posizione -->%d\n---Valore -->%d\n\n", getpid(),r->posizione,r->processo);

		disco[r->posizione]=r->processo;

		if((r->posizione)>old_pos)
			sleep_time=r->posizione-old_pos;
		else
			sleep_time=old_pos-r->posizione;
		
		sleep(sleep_time);

	}
	
	exit(0);
}
