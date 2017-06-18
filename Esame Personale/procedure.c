#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

void gestore(int id_richieste,int id_risposte){


	srand(time(NULL));

	msg_richiesta *msg_in=(msg_richiesta *) malloc(sizeof(msg_richiesta));

	msg_risposta *msg_out=(msg_risposta *) malloc(sizeof(msg_risposta));

	Magazzino magazzino[DIM_MAG];

	int i;
	
	for( i=0;i<DIM_MAG;i++){
		magazzino[i].costo=1+(rand()%5);
		magazzino[i].quantita=1+(rand()%5);
	}
	
	int res;
	int j;
	
	for( j=0;j<13;j++){

		res=msgrcv(id_richieste,msg_in,sizeof(msg_richiesta)-sizeof(long),0,0);
			if(res<0){
				printf("Errore Gestore nel ricevere la richiesta \n\n");
				exit(-1);
			}

		if(msg_in->tipo_richiesta==STATO_MAGAZZINO){
			msg_out->type=msg_in->type;

			msg_out->tipo_risposta=STATO_MAGAZZINO;

			printf("[GESTORE] Ricevuta richiesta di visualizzazione Stato da %d \n\n",msg_out->type);

			for(i=0;i<DIM_MAG;i++){
				
				msg_out->mag[i].costo=magazzino[i].costo;
				printf("******* %d",msg_out->mag[i].costo);
				msg_out->mag[i].quantita=magazzino[i].quantita;
			}

			res=msgsnd(id_risposte,msg_out,sizeof(msg_risposta)-sizeof(long),0);
				if(res<0){
					printf("Errore Gestore nell'invio risposta \n\n");
					exit(-1);
				}
			
		}

		else if(msg_in->tipo_richiesta==ACQUISTO){

			printf("[GESTORE] Ricevuta richiesta ACQUISTO da %d,\nArticolo =%d Quantita= %d \n\n",msg_in->type,msg_in->art.numero,msg_in->art.quantita);
 			msg_out->type=msg_in->type;
			msg_out->tipo_risposta=ACQUISTO;

			if(msg_in->art.quantita<(magazzino[msg_in->art.numero].quantita))
				magazzino[msg_in->art.numero].quantita=0;
			
			else
				magazzino[msg_in->art.numero].quantita=magazzino[msg_in->art.numero].quantita-msg_in->art.quantita;
	
			msg_out->art.quantita=msg_in->art.quantita;
			msg_out->art.numero=msg_in->art.numero;
			msg_out->art.costo=magazzino[msg_in->art.numero].costo;

			res=msgsnd(id_risposte,msg_out,sizeof(msg_risposta)-sizeof(long),0);
				if(res<0){
					perror("Errore Gestore nell'invio risposta ");
					exit(-1);
				}
			

		}
		else if(msg_in->tipo_richiesta==FORNITURA){
			printf("[GESTORE] Ricevuta FORNITURA da %d,\nArticolo =%d Quantita= %d \n\n",msg_in->type,msg_in->art.numero,msg_in->art.quantita);
			magazzino[msg_in->art.numero].quantita+=msg_in->art.quantita;
			
		}			
		else
			printf("[GESTORE]Richiesta non elaborata \n\n");
	
			
	}


}

void fornitore(int id_richieste){

	msg_richiesta *msg_out=(msg_richiesta *)malloc(sizeof(msg_richiesta));

	msg_out->type=getpid();
	msg_out->tipo_richiesta=FORNITURA;

	srand(time(NULL));

	int res;
	int i;	

	for( i=0;i<2;i++){
		
		msg_out->art.numero=rand()%10;
		msg_out->art.quantita=rand()%20;

		res=msgsnd(id_richieste,msg_out,sizeof(msg_richiesta)-sizeof(long),0);
			if(res<0){
					perror("Errore Fornitore nell'invio risposta ");
					exit(-1);
				}

		printf("[FORNITORE] {%d} Fornitura inviata Articolo =%d, Quantita =%d \n\n",getpid(),msg_out->art.numero,msg_out->art.quantita);
	
		sleep(2);	
	}

}

void cliente(int id_richieste,int id_risposte){

	srand(time(NULL));

	msg_richiesta *msg_out=(msg_richiesta *) malloc(sizeof(msg_richiesta));

	msg_risposta *msg_in=(msg_risposta *) malloc(sizeof(msg_richiesta));

	int res;

	msg_out->type=getpid();
	msg_out->tipo_richiesta=STATO_MAGAZZINO;
		res=msgsnd(id_richieste,msg_out,sizeof(msg_richiesta)-sizeof(long),0);
			if(res<0){
				
					perror("Errore CLIENTE nell'invio richiesta ");
					exit(-1);
			}
	
	
	res=msgrcv(id_risposte,msg_in,sizeof(msg_risposta)-sizeof(long),getpid(),0);
		if(res<0){
				
				perror("Errore CLIENTE nella ricezione ");
				exit(-1);
		}
		

	printf("[CLIENTE] Stato magazzino ricevuto \n");
	
	int i;	

	for( i=0;i<DIM_MAG;i++){
		printf("Articolo =%d, Quantita =%d, Costo =%d \n",i,msg_in->mag[i]	.quantita,msg_in->mag[i].costo);		
	}

	sleep(1);

	int pid=getpid();

	for(i=0;i<2;i++){

		msg_out->type=getpid();
		msg_out->tipo_richiesta=ACQUISTO;
		msg_out->art.numero=rand()%10;		
		msg_out->art.quantita=1+rand()%5;

		res=msgsnd(id_richieste,msg_out,sizeof(msg_richiesta)-sizeof(long),0);
			if(res<0){
				
					perror("Errore CLIENTE nell'invio richiesta ");
					exit(-1);
			}
	 	printf("%d aspetto risposta \n\n",pid);
		res=msgrcv(id_risposte,msg_in,sizeof(msg_risposta)-sizeof(long),pid,0);
			if(res<0){
				
					perror("Errore CLIENTE nella ricezione ");
					exit(-1);
			}

		printf("[CLIENTE] {%d} Ricevuta Conferma acquisto Articolo =%d, Quantita =%d Costo =%d \n\n",getpid(),msg_in->art.numero,msg_in->art.quantita,msg_in->art.costo);
	
		sleep(1);	
	}
	
}
