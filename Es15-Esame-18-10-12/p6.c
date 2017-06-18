#include "header.h"

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>

#include <stdio.h>
#include <stdlib.h>

int main(){
	key_t op_key=ftok(FTOK_PATH,FTOK_CHAR_Q_OP_2);
		int id_op=msgget(op_key,0);
			if(id_op<0){
				perror("ERRORE msgget");
				exit(-1);
			}
	
	key_t ris_key=ftok(FTOK_PATH,FTOK_CHAR_Q_RIS_2);
		int id_ris=msgget(ris_key,0);
			if(id_ris<0){
				perror("ERRORE msgget");
				exit(-1);
			}

	msg_operandi msg_op;
	msg_risposta msg_ris;
	
	int res;
	
	for(int i=0;i<3;i++){
		res=msgrcv(id_op,&msg_op,(sizeof(msg_operandi)-sizeof(long)),P6,0);
			if(res<0){
				perror("[P6] Errore ricezione messaggio");
				exit(-1);
			}
		
		printf("[P6] Messaggio ricevuto da P3 e=%d, f=%d\n\n",
				msg_op.operandi[0],
				msg_op.operandi[1]);
		
		int ris=msg_op.operandi[0]+msg_op.operandi[1];
		
		msg_ris.processo=P6;
		msg_ris.risposta=ris;
		
			res=msgsnd(id_ris,&msg_ris,(sizeof(msg_risposta)-sizeof(long)),0);
				if(res<0){	
					perror("[P6] Errore invio messaggio");
					exit(-1);	
				}
		printf("[P6] Risposta inviata a P3\n\n");
	}
	exit(0);
}
