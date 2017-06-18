/* Main.c */

#include "procedure.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main()
{
	MonitorElaborazioni *m=(MonitorElaborazioni *)malloc(sizeof(MonitorElaborazioni));
	InitMonitor(m);
	
	int ret;

	srand(getpid()+time(NULL));	

	pthread_attr_t att;
	pthread_t threads_prod[4];
	pthread_t threads_cons[2];	

	pthread_attr_init(&att);
	pthread_attr_setdetachstate(&att,PTHREAD_CREATE_JOINABLE);

	for(int i=0;i<4;i++){

		ret=pthread_create(&threads_prod[i],&att,Richiedente,(void *)m);	
		if(ret){
			printf("Errore nella creazione del thread \n");
			exit(-1);
		}
	}

	for(int i=0;i<2;i++){
		ret=pthread_create(&threads_cons[i],&att,Elaboratore,(void *)m);
		if(ret){
			printf("Errore nella creazione del thread \n");
			exit(-1);
		}
	}	
	
	for(int i=0;i<4;i++){
		ret=pthread_join(threads_prod[i],NULL);	
		if(ret){
			printf("Errore nel join del thread \n");
			exit(-1);
		}
	}

	for(int i=0;i<2;i++){
		ret=pthread_join(threads_cons[i],NULL);
		if(ret){
			printf("Errore nel join del thread \n");
			exit(-1);
		}
	}	

	DestroyMonitor(m);
}	
