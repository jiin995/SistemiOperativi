#include "lib.h"
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>

void invio(key_t key,char *ms)
{
	mess *m;
	int ds_mem;
	ds_mem=shmget(key,sizeof(mess),IPC_CREAT|0664);
	m=(mess *)shmat(ds_mem,0,0);
	m->ricevuto=1;
	m->msg=*(ms);
}

void ricevi(key_t key,char *ms)
{
	mess *m;
	int ds_mem,i;
	bool t=false;
	do
		{
			ds_mem=shmget(key,sizeof(mess),IPC_CREAT|IPC_EXCL|0664);
			if(ds_mem<0)
				t=true;
		}
	while(!t);
	ds_mem=shmget(key,sizeof(mess),0664);

	m=(mess*)shmat(ds_mem,0,0);

	while(m->ricevuto!=1);

	*(ms)=m->msg;
	shmctl(ds_mem,IPC_RMID,0);
}

