#include "procedure.h"
#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void Fornitore(Monitor *m,int *liv_scorte,int *scaffali_liberi,Magazzino *magazzino)
{
	for (int i=0;i<15;i++){
		int k=0;		

		enter_monitor(m);		

		while( (*scaffali_liberi)==0)
			wait_condition(m,OK_PROD);
		
		while((k<DIM_SCAFFALE) && (((*magazzino)[k].stato==OCCUPATO)||((*magazzino)[k].stato==IN_USO)))
			k++;
		
		(*magazzino)[k].stato=IN_USO;
		(*scaffali_liberi)--;

		leave_monitor(m);
		
		sleep(2);
		
		printf("[FORNITORE] {%d}: sto producendo nello scaffale %d \n",getpid(),k);
		
			
		(*magazzino)[k].id_fornitore=getpid();
		
		enter_monitor(m);
		
			(*magazzino)[k].stato=OCCUPATO;
			(*liv_scorte)++;
		
		signal_condition(m,OK_CONS);
		printf("magazzino[%d]==>stato=%d ==>valore=%d\n\n",i,(*magazzino)[k].stato,(*magazzino)[k].id_fornitore);	
		
	}
}

void Cliente (Monitor *m,int *liv_scorte,int *scaffali_liberi,Magazzino *magazzino)
{
	for(int i=0;i<15;i++){
		int k=0;
		int cons=0;

		enter_monitor(m);
		
		while((*liv_scorte)==0)
			wait_condition(m,OK_CONS);
		
		while((k<DIM_SCAFFALE) &&(((*magazzino)[k].stato==LIBERO)||((*magazzino)[k].stato==IN_USO)))
			k++;
		
		(*magazzino)[k].stato=IN_USO;
		(*liv_scorte)--;
		cons=(*magazzino)[k].id_fornitore;
		
		leave_monitor(m);
		
		sleep(2);
		
		printf("[CLIENTE] {%d}: sto consumando nello scaffale %d \n",getpid(),k);


		enter_monitor(m);
		
			(*magazzino)[k].stato=LIBERO;
			(*scaffali_liberi)++;
		
		signal_condition(m,OK_PROD);		
		
		printf("magazzino[%d]==>stato=%d ==>valore=%d\n\n",i,(*magazzino)[k].stato,cons);
		
	}
	
}
