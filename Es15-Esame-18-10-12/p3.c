#include "header.h"

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include <stdio.h>
#include <stdlib.h>

int main(){
	
	key_t op_1_key=ftok(FTOK_PATH,FTOK_CHAR_Q_OP_1);
		int id_op_1=msgget(op_1_key,0);
			if(id_op_1<0){
				perror("ERRORE msgget");
				exit(-1);
			}
	
	key_t op_2_key=ftok(FTOK_PATH,FTOK_CHAR_Q_OP_2);
		int id_op_2=msgget(op_2_key,IPC_CREAT|0664);
			if(id_op_2<0){
				perror("ERRORE msgget");
				exit(-1);
			}
		
	key_t ris_1_key=ftok(FTOK_PATH,FTOK_CHAR_Q_RIS_1);
		int id_ris_1=msgget(ris_1_key,0);
			if(id_ris_1<0){
				perror("ERRORE msgget");
				exit(-1);			
			}
	key_t ris_2_key=ftok(FTOK_PATH,FTOK_CHAR_Q_RIS_2);
		int id_ris_2=msgget(ris_2_key,IPC_CREAT|0664);
			if(id_ris_2<0){
				perror("ERRORE msgget");
				exit(-1);
			}
	
	msg_operandi msg_op,msg_op_1;
	msg_risposta msg_ris;
	
	int r1,r2;	
	
	for(int i=0;i<3;i++){
		
		int res;

		res=msgrcv(id_op_1,&msg_op,(sizeof(msg_op)-sizeof(long)),P3,0);
			if(res<0){
				perror("[P3] Errore ricezione messaggio ");
				exit(-1);
			}

		printf("[P3] Messaggio ricevuto da P1 c=%d, d=%d, e=%d, f=%d \n\n",
				msg_op.operandi[0],
				msg_op.operandi[1],
				msg_op.operandi[2],
				msg_op.operandi[3]);
		
		msg_op_1.processo=P5;
		msg_op_1.operandi[0]=msg_op.operandi[0];
		msg_op_1.operandi[1]=msg_op.operandi[1];		
		
		printf("[P3] Invio messaggio a P5\n\n");
		
			res=msgsnd(id_op_2,&msg_op_1,(sizeof(msg_operandi)-sizeof(long)),0);
				if(res<0){
					perror("[P3] Errore invio messaggio ");
					exit(-1);
				}

		msg_op_1.processo=P6;
		msg_op_1.operandi[0]=msg_op.operandi[2];
		msg_op_1.operandi[1]=msg_op.operandi[3];		
		
		printf("[P3] Invio messaggio a P6\n\n");
		
			res=msgsnd(id_op_2,&msg_op_1,(sizeof(msg_operandi)-sizeof(long)),0);
				if(res<0){
					perror("[P3] Errore invio messaggio ");
					exit(-1);
				}
		
		printf("[P3] Aspetto messaggio da P5\n\n");
		
			res=msgrcv(id_ris_2,&msg_ris,(sizeof(msg_risposta)-sizeof(long)),P5,0);
				if(res<0){
					perror("[P3] Errore ricezione messaggio ");
					exit(-1);
				}
	
		r1=msg_ris.risposta;		
		
		printf("[P3] Messaggio ricevuto da P5 ==>%d \n\n",msg_ris.risposta);		

		printf("[P3] Aspetto messaggio da P6\n\n");
		
			res=msgrcv(id_ris_2,&msg_ris,(sizeof(msg_risposta)-sizeof(long)),P6,0);
				if(res<0){
					perror("[P3] Errore ricezione messaggio ");
					exit(-1);
				}
		
		r2=msg_ris.risposta;
		
		printf("[P3] Messaggio ricevuto da P6 ==>%d\n\n",msg_ris.risposta);	

		printf("[P3] Invio messaggio a P1 \n\n");
		
		msg_ris.processo=P3;
		msg_ris.risposta=r1*r2;
			
		msgsnd(id_ris_1,&msg_ris,(sizeof(msg_risposta)-sizeof(long)),0);
		
		printf("[P3] Risposta inviata a P1 \n\n");
	}

	msgctl(id_op_2,IPC_RMID,0);
	msgctl(id_ris_2,IPC_RMID,0);

	exit(0);
}
