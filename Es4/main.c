/* Realizzare un programma che crei un produttore e un consumatore il produttore legge un numero da tastiera e come lo legge lo comunica al consumatore 
	quando al produttore viene inserito 0 termina la sua esecuzione e il consumatore fa la media dei valori letti */

#include "procedure.h"

int main()
{
	key_t sem=ftok("./",'f');
	key_t shm=ftok("./",'v');
	
	int pid=fork();
	
	if(pid<0){
			perror("Errore nell' esecuzione della FORK \n");
			exit(-1);
		}
	if(pid==0){
			produttore(shm,sem);
			exit(1);
		}
	
	//sleep(1);
	
	printf("\nOra creo il consumatore \n");
	pid=fork();
	
	if(pid<0){
			perror("Errore nell' esecuzione della FORK \n");
			exit(-1);
		}
	if(pid==0){
			consumatore(shm,sem);
			exit(1);
		}
	wait(NULL);
	wait(NULL);	
	printf("Processo Produttore e Processo Consumatore hanno terminato la loro esecuzione rimossa la SHM e i SEM \n\n");
	
	
	
	
	return 0;
}
