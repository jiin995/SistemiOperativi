#include "procedure.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

void InitMonitor(MonitorElaborazioni *mon)
{
	for(int i=0;i<BUFFER_DIM;i++){
		mon->stato[i]=VUOTO;	
	}
	
	mon->liberi=BUFFER_DIM;
	mon->pieni=0;	

	pthread_mutex_init(&mon->m,NULL);
	pthread_cond_init(&mon->ok_elab,NULL);
	pthread_cond_init(&mon->ok_prod,NULL);
}

void DestroyMonitor(MonitorElaborazioni *mon)
{
	pthread_mutex_destroy(&(mon->m));
	pthread_cond_destroy(&(mon->ok_elab));
	pthread_cond_destroy(&(mon->ok_prod));
}

void Produci(MonitorElaborazioni *monitor,buffer *buf)
{
	int i=0;
	int tr=0;
	pthread_mutex_lock(& (monitor->m));
	
	while(monitor->liberi==0){
		pthread_cond_wait(&(monitor->ok_prod),&(monitor->m));
	}

	while(!tr){
		if(monitor->stato[i++]==VUOTO){
			monitor->liberi--;			
			monitor->stato[--i]=IN_USO;	
			tr=1;
		}
	}	
		/*while(!tr)	{
				if(monitor->stato[i++]==VUOTO)
					tr=1;
				else if(i==BUFFER_DIM){
					pthread_cond_wait(&(monitor->ok_prod),&(monitor->m));
					i=0;
				}						
			}*/	
	pthread_mutex_unlock(& (monitor->m));	
	
	printf("[debug] producendo in [%d]\n\n", i);

	monitor->elaborazione[i].operandi_tot=buf->operandi_tot;
	
	for(int j=0;j<buf->operandi_tot;j++){
		monitor->elaborazione[i].operandi[j]=buf->operandi[j];	
	}
		
	pthread_mutex_lock(&(monitor->m));


		monitor->stato[i]=PIENO;	
		monitor->pieni++;
		pthread_cond_signal(& (monitor->ok_elab));

	pthread_mutex_unlock(&(monitor->m));
}	

int Elabora(MonitorElaborazioni *monitor,buffer *bu)
{
	
		

	
	int min=-1;
	int op_min=-1;
	
	pthread_mutex_lock(& (monitor->m));		

	while(monitor->pieni==0){
		pthread_cond_wait(& (monitor->ok_elab),& (monitor->m));
	}

	for(int i=0;i<BUFFER_DIM;i++){
		if((monitor->stato[i]==PIENO) && (min==-1 || monitor->elaborazione[i].operandi_tot<op_min))
		{
			min=i;
			op_min=monitor->elaborazione[i].operandi_tot;
		}
	}

	monitor->stato[min]=IN_USO;

	monitor->pieni--;
	pthread_mutex_unlock(&(monitor->m));
	
	printf("[debug] consumando in [%d]\n\n", min);	
	
	sleep(bu->operandi_tot);
	
	bu->operandi_tot=monitor->elaborazione[min].operandi_tot;

	int somma=0;
	int i=0;
	for(i=0;i<(bu->operandi_tot);i++){
		bu->operandi[i]=monitor->elaborazione[min].operandi[i];
		somma =somma+bu->operandi[i];
	}
	
	printf("[debug]  prelievo %d, totop=%d, op1=%d, op2=%d, op3=%d, op4=%d ris=%d\n\n",
			min,	
			bu->operandi_tot,		
			bu->operandi[0],
			bu->operandi[1],
			(bu->operandi_tot>=3 ? bu->operandi[2] :-1),
			(bu->operandi_tot>=4 ? bu->operandi[3] :-1),
			somma);	


	
	pthread_mutex_lock(& (monitor->m));
	
		monitor->liberi++;
		monitor->stato[min]=VUOTO;
		pthread_cond_signal(& (monitor->ok_prod));
	
	pthread_mutex_unlock(& (monitor->m));
	
	return somma;
}

void *Richiedente(void *in)
{
	MonitorElaborazioni *m=(MonitorElaborazioni *)in;
	
	for(int i=0;i<3;i++){
		buffer b;
		b.operandi_tot=2+rand()%3;
		
		for(int j=0;j<b.operandi_tot;j++){
			b.operandi[j]=rand()%11;		
		}
	Produci(m,&b);
	
	printf("[PRODUTTORE] nop=%d, op1=%d, op2=%d, op3=%d, op4=%d\n\n",
			b.operandi_tot,		
			b.operandi[0],
			b.operandi[1],
			(b.operandi_tot>=3 ? b.operandi[2] :-1),
			(b.operandi_tot>=4 ? b.operandi[3] :-1));
	}
	
	pthread_exit(0);
}

void *Elaboratore(void *in)
{
	MonitorElaborazioni *m=(MonitorElaborazioni *)in;

	
	for(int i=0;i<6;i++){
		buffer b;
		int r;
		r=Elabora(m,&b);
		printf("[CONSUMATORE] nop=%d, op1=%d, op2=%d, op3=%d, op4=%d, ris=%d\n\n",
			b.operandi_tot,		
			b.operandi[0],
			b.operandi[1],
			(b.operandi_tot>=3 ? b.operandi[2] :-1),
			(b.operandi_tot>=4 ? b.operandi[3] :-1),
			r);
	}
	

	pthread_exit(0);
}

