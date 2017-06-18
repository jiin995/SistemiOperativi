#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc,char *argv[]){
	
	if(argc!=3){
		printf("Errore Richiamo funzione numero parametri non valido :%d\n\n",argc);
		exit(-1);
	}
	
	char char_mi,char_de;
	
	char_mi=(char) *argv[1];
	char_de=(char) *argv[2];
	
	key_t key_mit,key_dest;
	int id_mit,id_des;
				
	key_mit=ftok(PATH,char_mi);
		id_mit=msgget(key_mit,IPC_CREAT|IPC_EXCL|0664);
		if(id_mit==-1){
			printf("[CHAT MITTENTE] {%d} Coda mittente gia' esistente effettuo l'attach \n\n",getpid());
			id_mit=msgget(key_mit,0);
		}
	
	key_dest=ftok(PATH,char_de);
		id_des=msgget(key_dest,IPC_CREAT|IPC_EXCL|0664);
		if(id_des==-1){
			printf("[CHAT MITTENTE] {%d} Coda destinazione gia' esistente effettuo l'attach \n\n",getpid());
			id_des=msgget(key_dest,0);
		}

	int pid_mittente,pid_destinatario,pid;
	
	pid_mittente=fork();
		if(pid_mittente==0){
			execve("mittente",argv,0);
		}		
		
	pid_destinatario=fork();
		if(pid_destinatario=0){
			execve("ricevente",argv,0);
		}
	
	for(int i=0;i<2;i++){
		pid=wait(&NULL);
	
		printf("Processo {%d} Terminato \n\n",pid);
	}

	msgctl(id_mit,IPC_RMID,0);
	msgctl(id_des,IPC_RMID,0);
}
