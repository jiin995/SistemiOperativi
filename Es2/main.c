#include "lib.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>

int main()
{
	char m;	
	key_t k=ftok("./",'a');	
	int pid=fork();
	if(pid>0)
		{
			printf("Sono il processo padre con pid %d \n",getpid());
			printf("Inserisci il carattere da iviare \n");
			scanf("%c",&m);
			invio(k,&m);
			return 0;
		}
	else if(pid==0)
		{
			printf("Sono il figlio con pid %d\n",getpid());
			sleep(1);
			ricevi(k,&m);
			printf("Messaggio ricevuto %c\n",m);
			exit(0);
		}
	else 
		exit(-1);
	return 0;
}
