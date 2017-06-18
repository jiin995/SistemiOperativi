#include "procedure.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>



int main()
{
	param parametri[N_THREAD];

	GestioneVoli *gest=(GestioneVoli *) malloc(sizeof(GestioneVoli));
	
	pthread_t threads[N_THREAD];	
	pthread_attr_t att;
	
	pthread_attr_init(&att);
	pthread_attr_setdetachstate(&att,PTHREAD_CREATE_JOINABLE);	
	
	InitMonitor(gest);
	int res=0;

	for(int i=0;i<N_THREAD;i++){
		parametri[i].id=i;
		parametri[i].gest=gest;
		pthread_create(&threads[i],&att,ControlloreVolo,(void *) & parametri[i]);
		if(res){
			printf("[ERRORE] Impossibile creare il THREAD \n\n%d");
			exit(-1);
		}		
	}	
	
	for(int i=0;i<N_THREAD;i++){
		pthread_join(threads[i],NULL);
	}
	
	DestroyMonitor(gest);	
	//free(gest);
}
