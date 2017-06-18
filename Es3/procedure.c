#include "procedure.h"

int execback (char *programma,char *args[])
{
	int pidback=fork();
		if(pidback<0)
			return -1;
		else if(pidback>0) 	//Se sono la shell aspetto che mi ritorni l'esecuzione
			wait(NULL);
		else
			{   		//Sono il figlio ma devo subito ritornare il controllo a mio padre
				printf("Sono il figlio \n");
				int pidb=fork();
				if(pidb>0)
					_exit(1);	//Il figlio ritorna il controllo al padre 
				else if(pidb==0) 	//Sono il nipote e devo eseguire in background
					execvp(programma,args);
				else 
					_exit(-1);
			}
} 

int exnoback (char * programma,char *args[])
{
	int pidback=fork();
                if(pidback<0)
                        return -1;
                else if(pidback>0)      //Se sono la shell aspetto che mi ritorni l'esecuzione
                        wait(NULL);
                else
			execvp(programma,args);
}
