#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int pid,st;
	
	pid=fork();
	
	if(pid==-1)
		{
			printf("Il processo non e' stato creato");
			return(-1);
		}
	if(pid>0)
		{
			printf("Sono il processo padre e attendo la terminazione del  figlio\n");
			printf("Il mio pid : %d \n",getpid());
			wait(&st);
			printf("Il processo figlio ha terminato la sua esecuzione e ha ritornato\n");
			if((char) st==0)
				printf("Terminato volantariamente con return =%d\n",st>>8);
			else 
				printf("Processo terminato involontareamente\n");
		}
	if(pid==0)
		{
			printf("Sono il processo figlio con il pid %d\n",getpid());
			printf("Il processo padre ha pid :%d\n ",getppid());
			sleep(4);
			_exit(3);
		}
	return 0;			
}	
