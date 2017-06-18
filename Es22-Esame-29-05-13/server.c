//Server

#include "header.h"

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_DIM 10
#define N_THREAD 2

typedef struct{
	
	pthread_mutex_t mutex;
	
	pthread_cond_t ok;	
	pthread_cond_t ok_in;		

	msg_richiesta Buffer[BUFFER_DIM];
	int testa;
	int coda;

	int msg_presenti;	

	int id_msg_risp;
}input_struct;

void *Worker(void *in){

	input_struct *input=(input_struct *)in;
	
	msg_risposta *risp=(msg_risposta *)malloc(sizeof(msg_risposta));
	int res;
	
	printf("[WORKER AVVIATO]\n\n");

	while(1){
		
		pthread_mutex_lock(&(input->mutex));
		
			while(input->msg_presenti==0)
				pthread_cond_wait(&(input->ok),&(input->mutex));

			input->msg_presenti--;

			risp->ris=(input->Buffer[input->coda].v1)*(input->Buffer[input->coda].v2);
			risp->type=input->Buffer[input->coda].type;

			input->coda=(input->coda +1)%BUFFER_DIM;
	
			pthread_cond_signal(&(input->ok_in));

		pthread_mutex_unlock(&(input->mutex));	

		res=msgsnd(input->id_msg_risp,risp,(sizeof(msg_risposta)-sizeof(long)),0);

			if(res<0){
				printf("[THREAD WORKER] Erorre invio messaggio \n\n");
				pthread_exit(0);
			}
	}

}

int main(int argc,char *argv[]){

	key_t key_richieste=ftok(FTOK_PATH,FTOK_CHAR_RIC);
		int id_richieste=msgget(key_richieste,IPC_CREAT|IPC_EXCL|0664);
			if(id_richieste<0)
				id_richieste=msgget(key_richieste,0664);
	
	key_t key_risposte=ftok(FTOK_PATH,FTOK_CHAR_RIS);
		int id_risposte=msgget(key_risposte,IPC_CREAT|IPC_EXCL|0664);
			if(id_risposte<0)
				id_risposte=msgget(key_risposte,0664);
	
	int i;

	printf("[SERVER] {%d} avviato \n\n",getpid());
	
	pthread_t thread[N_THREAD];
	pthread_attr_t att;
	
	pthread_attr_init(&(att));
	pthread_attr_setdetachstate(&(att),PTHREAD_CREATE_JOINABLE);
	
	input_struct *input=(input_struct *)malloc(sizeof(input_struct));
	
	input->coda=0;
	input->testa=0;
	input->msg_presenti=0;
	input->id_msg_risp=id_risposte;
	
	pthread_mutex_init(&(input->mutex),0);
	
	pthread_cond_init(&(input->ok),0);
	pthread_cond_init(&(input->ok_in),0);

	for(i=0;i<N_THREAD;i++)
		pthread_create(&(thread[i]),&att,Worker,(void *) input);

	msg_richiesta *msg=(msg_richiesta*)malloc(sizeof(msg_richiesta));	

	int tr=1;
	int res;

	while(tr){

		res=msgrcv(id_richieste,msg,sizeof(msg_richiesta)-sizeof(long),0,IPC_NOWAIT);
			if(res>=0){
				printf("[MANAGER] Messaggio ricevuto \nPID :%d v1= %d  v2=%d\n\n",msg->type,msg->v1,msg->v2);
		
				if((msg->v1==-1)&&(msg->v2==-1))
					tr=0;
				else{
					pthread_mutex_lock(&(input->mutex));
	
						while(input->msg_presenti==BUFFER_DIM)
							pthread_cond_wait(&(input->ok_in),&(input->mutex));
							
						input->Buffer[input->testa].type=msg->type;
						input->Buffer[input->testa].v1=msg->v1;
						input->Buffer[input->testa].v2=msg->v2;

						input->testa=(input->testa+1)%BUFFER_DIM;
						input->msg_presenti++;
				
						pthread_cond_signal(&(input->ok));
				
					pthread_mutex_unlock(&(input->mutex));
					printf("[MANAGER] messaggio inviato \n\n");
				}	

			}
		sleep(1);
	}

	for(i=0;i<N_THREAD;i++)
		pthread_cancel(thread[i]);

	return 0;
}

	
	
	
