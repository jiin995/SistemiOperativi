#include "procedure.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <sys/msg.h>
#include <pthread.h>

extern int inbox_Recive;

int Client(int send_Inbox,int recive_Inbox)
{
	msgs msg_Sended;
	msgr msg_Recived;
	int res;
	printf("client %d",getpid());
	for(int count=0;count<5;count++)
		{
			msg_Sended.type=getpid();
			srand(getpid()+rand());
			msg_Sended.n1=rand()%101;
			msg_Sended.n2=rand()%101;
				res=msgsnd(send_Inbox,&msg_Sended,sizeof(msgs)-sizeof(long),IPC_NOWAIT);

			if (res <0)
				return -1; 	//Client error msg_Send

			res=msgrcv(recive_Inbox,&msg_Recived,sizeof(msgr)-sizeof(long),getpid(),0);

			if (res<0)
				return -2;	//Client error msg_Recive
			printf("[Client] :%d	Messaggio :%d\n",getpid(),count);
			printf("[Client] Messaggio ricevuto dal Server il risultato di %d x %d =%d \n",msg_Sended.n1,msg_Sended.n2,msg_Recived.result);

		}
}

/* la send inbox e la recive inbox del server sono le inverse di quelle del client */

int Server_Main(int send_Inbox,int recive_Inbox)
{
	pthread_t threads; //Non mi interessa per ora tenere traccia dei singoli thread che lancio
	int res;
	int i=0;
	pthread_mutex_init(&mutex_Sendbox,NULL);
	//input_Struct input;
	//	input.send_Inbox=send_Inbox;
	while(1){
		msgs *msg_Recived=(msgs *)malloc(sizeof(msgs));
       	 	msg_Recived->n1=0;
		msg_Recived->n2=0;
        	msg_Recived->type=0;
		res=msgrcv(recive_Inbox,msg_Recived,sizeof(msgs)-sizeof(long),0,0); //Recive bloccante che accetta ogni tipo di msg

		if(res<0)
			return -3;		//Server error msg_Recive
		if((msg_Recived->n1==-1)&&(msg_Recived->n2==-1))
			return 0;
		else{
			printf("[Server] messaggio ricevuto da %d, n1= %d, n2=%d \n",msg_Recived->type,msg_Recived->n1,msg_Recived->n2);
			//input.message=*msg_Recived;
			res=pthread_create(&threads,NULL,Server_Thread,(void*) msg_Recived);
		}

	}
}

void* Server_Thread(void* input)
{
	msgs *in=(msgs*)(input);

	msgr response;

	response.type=in->type;
	response.result=(in->n1)*(in->n2);

	pthread_mutex_lock(&mutex_Sendbox);
				printf("[Thread] %d, n1 =%d, n2 =%d, ris =%d\n",in->type,in->n1,in->n2,in->n1*in->n2);
		msgsnd(inbox_Recive,&response,sizeof(msgr)-sizeof(long),0);
	pthread_mutex_unlock(&mutex_Sendbox);

	free(in);

	pthread_exit(NULL);
}
