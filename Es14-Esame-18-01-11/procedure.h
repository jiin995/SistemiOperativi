/*********PROCEDURE.H********/

#ifndef PROCEDURE_H
#define PROCEDURE_H

#define TOT_SERVER 3
#define TOT_CLIENT 3
#define TOT_MSG   3 //15
#define MSG_SERVER 40

#include <sys/types.h>

typedef struct{
	long type;
	pid_t PID;
}msg_t;


void Balancer(int msg_id_balancer,int msg_id_server[]);
void Client(int msg_id_balancer);
void Server(int msg_id_balancer);

#endif
