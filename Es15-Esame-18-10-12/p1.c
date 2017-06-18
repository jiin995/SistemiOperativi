#include "header.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	
	printf("******Processo P1 avviato******\n");

	key_t op_key=ftok(FTOK_PATH,FTOK_CHAR_Q_OP_1);
		int id_msg_op1=msgget(op_key,0);
		
		if(id_msg_op1<0){
			perror("msgget FALLITA");
			exit(-1);
		}

	key_t ris_key=ftok(FTOK_PATH,FTOK_CHAR_Q_RIS_1);
		int id_msg_ris1=msgget(ris_key,0);
		
		if(id_msg_ris1<0){
			perror("msgget FALLITA");
			exit(-1);
		}

	srand(time(NULL));
	
	for(int i=0;i<3;i++){
		
		msg_operandi msg_op;
		msg_risposta msg_ris;
		
		int ret=0;
		
		int a=rand()%10;
		int b=rand()%10;
		int c=rand()%10;
		int d=rand()%10;
		int e=rand()%10;
		int f=rand()%10;
		int g=rand()%10;
		int h=rand()%10;
	
		int r1,r2,r3, risultato;

		printf("Operandi generati dal processo P1 a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d, h=%d \n\n",a,b,c,d,e,f,g,h);

		msg_op.processo=P2;
			msg_op.operandi[0]=a;
			msg_op.operandi[1]=b;
	
		printf("[P1] Invio operandi al processo P2 \n\n");
		
			ret=msgsnd(id_msg_op1,&msg_op,(sizeof(msg_operandi)-sizeof(long)),0);
				if(ret<0){
					perror("P1 Errore nell'invio del messaggio a P2 \n\n");	
					exit(-1);
				}
		
		msg_op.processo=P3;
			msg_op.operandi[0]=c;
			msg_op.operandi[1]=d;
			msg_op.operandi[2]=e;
			msg_op.operandi[3]=f;
		
		printf("[P1] Invio operandi al processo P3 \n\n");

			ret=msgsnd(id_msg_op1,&msg_op,(sizeof(msg_operandi)-sizeof(long)),0);
				if(ret<0){
					perror("P1 Errore nell'invio del messaggio a P3 \n\n");	
					exit(-1);
				}
		
		msg_op.processo=P4;
			msg_op.operandi[0]=g;
			msg_op.operandi[1]=h;
	
		printf("[P1] Invio operandi al processo P4\n\n");
		
			ret=msgsnd(id_msg_op1,&msg_op,(sizeof(msg_operandi)-sizeof(long)),0);
				if(ret<0){
					perror("P1 Errore nell'invio del messaggio a P4 \n\n");	
					exit(-1);
				}
		
		printf("[P1] Aspetto risultato da parte di P2\n\n");
		
			ret=msgrcv(id_msg_ris1,&msg_ris,(sizeof(msg_risposta)-sizeof(long)),P2,0);
				if(ret<0){
					perror("P1 Errore nel ricevere il messaggio da P2\n\n");	
					exit(-1);
				}
		printf("[P1] Risultato ottenuto da parte di P2 ==>%d\n\n",msg_ris.risposta);

			r1=msg_ris.risposta;
		
		printf("[P1] Aspetto risultato da parte di P3\n\n");
		
			ret=msgrcv(id_msg_ris1,&msg_ris,(sizeof(msg_risposta)-sizeof(long)),P3,0);
				if(ret<0){
					perror("P1 Errore nel ricevere il messaggio da P3\n\n");	
					exit(-1);
				}
		printf("[P1] Risultato ottenuto da parte di P3 ==>%d\n\n",msg_ris.risposta);		

			r2=msg_ris.risposta;
		
		printf("[P1] Aspetto risultato da parte di P4\n\n");
		
			ret=msgrcv(id_msg_ris1,&msg_ris,(sizeof(msg_risposta)-sizeof(long)),P4,0);
				if(ret<0){
					perror("P1 Errore nel ricevere il messaggio da P2\n\n");	
					exit(-1);
				}
		printf("[P1] Risultato ottenuto da parte di P4 ==>%d\n\n",msg_ris.risposta);		
			r3=msg_ris.risposta;

		risultato=r1+r2+r3;
	
		printf("[P1] Elaborazione correttamente avvenuta ==>risultato :%d risultato ricevuto :%d \n\n",( a*b + (c+d)*(e+f) + g*h ),risultato);
	
	}

	exit(0);
}
