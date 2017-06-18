#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>


#include <stdio.h>
#include <unistd.h>

#include "monitor.h"

//Funzioni di utilita' private della libreria

static void Wait_Sem(int,int);
static void Signal_Sem(int,int);
static int  Queue_Sem(int,int);

/*********IMPLEMENTAZIONE PROCEDURE MONITOR*********/

void init_monitor(Monitor *m,int n_cond)
{
	m->id_mutex=semget(IPC_PRIVATE,1,IPC_CREAT|0664);
		semctl(m->id_mutex,0,SETVAL,1);
	
	m->n_var_cond=n_cond;
	
	m->id_conds=semget(IPC_PRIVATE,n_cond,IPC_CREAT|0664);
		for(int i=0;i<n_cond;i++)
			semctl(m->id_conds,i,SETVAL,0);

	m->id_shm_cond_counts=shmget(IPC_PRIVATE,(n_cond*sizeof(int)),IPC_CREAT|0664);
	m->cond_counts=(int *)(shmat(m->id_shm_cond_counts,0,0));
		for(int i=0;i<n_cond;i++)
			m->cond_counts[i]=0;
	
}

void enter_monitor(Monitor *m)
{
	Wait_Sem(m->id_mutex,0);
}

void leave_monitor(Monitor *m)
{
	Signal_Sem(m->id_mutex,0);
}

void remove_monitor(Monitor *m)
{
	semctl(m->id_mutex,0,IPC_RMID,0);
	semctl(m->id_conds,m->n_var_cond,IPC_RMID,0);
	shmctl(m->id_shm_cond_counts,IPC_RMID,0);
}

void wait_condition(Monitor *m,int id_var)
{
	m->cond_counts[id_var]++;
	Signal_Sem(m->id_mutex,0);
		Wait_Sem(m->id_conds,id_var);
	m->cond_counts[id_var]--;
}

void signal_condition(Monitor *m,int id_var)
{
	if(m->cond_counts[id_var]!=0)
		Signal_Sem(m->id_conds,id_var);
	else
		Signal_Sem(m->id_mutex,0);
}

int queue_condition(Monitor *m,int id_var){
	return m->cond_counts[id_var];
}

/****************Imp Funzioni Semaforiche***************/

void Wait_Sem(int id_sem,int n_sem)
{
	struct sembuf sem_buf;
		sem_buf.sem_num=n_sem;
		sem_buf.sem_flg=0;
		sem_buf.sem_op=-1;
	semop(id_sem,&sem_buf,1);	
}

void Signal_Sem(int id_sem,int n_sem)
{
	struct sembuf sem_buf;
		sem_buf.sem_num=n_sem;
		sem_buf.sem_flg=0;
		sem_buf.sem_op=1;
	semop(id_sem,&sem_buf,1);
}

int Queue_Sem(int id_sem,int n_sem)
{
	return (semctl(id_sem,n_sem,GETNCNT,NULL));
}
