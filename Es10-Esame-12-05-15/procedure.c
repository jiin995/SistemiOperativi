#include "procedure.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern GestioneVoli *gest;

void InitMonitor (GestioneVoli *g)
{
	pthread_mutex_init(& (g->m),NULL);
	pthread_cond_init(& (g->ok_inser),NULL);

	g->in_volo=0;
	for(int i=0;i<N_VOLI;i++)
		g->stato_voli[i]=LIBERO;
}

void DestroyMonitor (GestioneVoli *g)
{
	pthread_mutex_destroy(& (g->m));
	pthread_cond_destroy(& (g->ok_inser));
}

void InserisciVolo(GestioneVoli *g,int ident)
{
	pthread_mutex_lock(& (g->m));
	
	while(g->in_volo==N_VOLI)
		pthread_cond_wait(& (g->ok_inser),& (g->m));	
	
	printf("[DEBUG] Inserisco Volo :%d\n\n",ident);

	int i=0;
	int tr=0;
	
	while(g->stato_voli[i]!=LIBERO)
		i++;

	g->stato_voli[i]=IN_USO;
	g->in_volo++;
	
	pthread_mutex_unlock(& (g->m));
	
	sleep(1);
	
		g->vett_voli[i].identificativo=ident;
		g->vett_voli[i].quota=0;
		
	pthread_mutex_lock(& (g->m));
		
		g->stato_voli[i]=OCCUPATO;
	
	pthread_mutex_unlock(&(g->m));
	
	printf("[DEBUG] Inserimento Volo :%d in posizione :%d Effettuato Correttamente\n\n",g->vett_voli[i].identificativo,i);
		
}

void RimuoviVolo (GestioneVoli *g,int ident)
{
	pthread_mutex_lock(& (g->m));
	
	int i=0;
	
		while((g->vett_voli[i].identificativo !=ident)){
			i++;
			if(i==N_VOLI)
				i=0;	
		}

		g->stato_voli[i]=IN_USO;
	
	pthread_mutex_unlock(& (g->m));
	
	sleep(1);
	
	pthread_mutex_lock(& (g->m));
	
		g->stato_voli[i]=LIBERO;
		g->in_volo--;
		pthread_cond_signal(&(g->ok_inser));
	
	pthread_mutex_unlock(& (g->m));	
	
	printf("[DEBUG] Rimozione Volo :%d in posizione :%d Effettuato Correttamente\n\n",g->vett_voli[i].identificativo,i);
	
}

void AggiornaVolo(GestioneVoli *g,int ident,int quota)
{
	pthread_mutex_lock(& (g->m));
	
	int i=0;
	int id_buffer=-1;

		while(i<N_VOLI && id_buffer==-1){	
			i++;
			if((g->stato_voli[i]==OCCUPATO) && (g->vett_voli[i].identificativo==ident)){
				id_buffer=i;				
			}
		}
	g->stato_voli[id_buffer]=IN_USO;
	
	pthread_mutex_unlock(& (g->m));

	sleep(rand()%4);
	
	g->vett_voli[id_buffer].quota=quota;
	
	pthread_mutex_lock(& (g->m));
		
		g->stato_voli[id_buffer]=OCCUPATO;
	
	pthread_mutex_unlock(& (g->m));
	
	printf("[DEBUG] Aggiornamento Volo :%d in posizione :%d Nuova quota :%d Effettuato Correttamente\n\n",g->vett_voli[id_buffer].identificativo,id_buffer,g->vett_voli[id_buffer].quota);
}

void * ControlloreVolo(void *in)
{
	//int *ident=(int *)malloc(sizeof(int));
	
	param *parametri=(param*) in;

	GestioneVoli *g=parametri->gest;
	int id=parametri->id;	
	
	printf("Controllore di volo %d\n\n",parametri->id);	

	InserisciVolo(g,id);
	
	AggiornaVolo(g,id,1000);
	AggiornaVolo(g,id,2000);
	AggiornaVolo(g,id,1000);

	RimuoviVolo(g,id);

}


