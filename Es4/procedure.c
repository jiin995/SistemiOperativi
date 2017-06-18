#include "procedure.h" 

//Sem 0 segnalo che ho prodotto
//Sem 1 segnalo che il produttore ha fatto 5 produzioni

int wait_sem(int sem_id,int sem_num)
{
	struct sembuf sem_buf;
		sem_buf.sem_num=sem_num;
		sem_buf.sem_flg=0;
		sem_buf.sem_op=-1;
	 return semop(sem_id,&sem_buf,1);
}

int signal_sem(int sem_id,int sem_num)
{
	struct sembuf sem_buf;
		sem_buf.sem_num=sem_num;
		sem_buf.sem_flg=0;
		sem_buf.sem_op=1;
	return semop(sem_id,&sem_buf,1);
}

void produttore(key_t shm_key,key_t sem_key)
{
		
	int id_shmem=shmget(shm_key,sizeof(int),IPC_CREAT|0664|IPC_EXCL);

	if(id_shmem < 0) {
		id_shmem=shmget(shm_key,sizeof(int),0664);
			if(id_shmem<0){
				perror("Errore SHMGET");
				exit(1);
			}
	}
	else
		{
	
			printf("Produttore --> Creato la SHM\n");
		}
	
	int id_sem=semget(sem_key,2,IPC_CREAT|0664|IPC_EXCL);
	
	if(id_sem< 0) {
		id_sem=semget(sem_key,2,0664);
			if(id_sem<0){
				perror("Errore SEMGET");
				exit(1);
			}
	}
	else
		{
			semctl(id_sem,0,SETVAL,1);
			printf("Produttore -->SEMAFORO INIZIALIZZATO AD 1  consuma\n");
	
			semctl(id_sem,1,SETVAL,0);
			printf("Produttore --> SEMAFORO INIZIALIZZATO AD 0 fai media \n");
			printf("Produttore --> Creato la SHM\n");	
		}
	
	int *i= (int *)shmat(id_shmem,0,0);
	
	*i = 0;
	printf("VALORE INIZIALE: %d\n", *i);
	
	int rip=1;
	while(rip)
		{	
			wait_sem(id_sem,0);
			printf("Produttore --> Inserisci valore =");
			scanf("%i",i);
			if(*i==0)
				rip=0;
			signal_sem(id_sem,0);
		}
	signal_sem(id_sem,1);
}

void consumatore (key_t shm_key,key_t sem_key)
{
	int id_shmem=shmget(shm_key,sizeof(int),IPC_CREAT|0664|IPC_EXCL);

	if(id_shmem < 0) {
		id_shmem=shmget(shm_key,sizeof(int),0664);
			if(id_shmem<0){
				perror("Errore SHMGET");
				exit(1);
			}
	}
	else
		{
	
			printf("Consumatore --> Creato la SHM\n");
		}

	int id_sem=semget(sem_key,2,IPC_CREAT|0664|IPC_EXCL);
	
	if(id_sem< 0) {
		id_sem=semget(sem_key,2,0664);
			if(id_sem<0){
				perror("Errore SEMGET");
				exit(1);
			}
	}
	else
		{
			semctl(id_sem,0,SETVAL,1);
			printf("Consumatore -->SEMAFORO INIZIALIZZATO AD 1  consuma\n");
	
			semctl(id_sem,1,SETVAL,0);
			printf("Consumatore --> SEMAFORO INIZIALIZZATO AD 0 fai media \n");
			printf("Consumatore --> Creato la SEM \n");	

		}
	int *i= (int *) shmat(id_shmem,0,0);
	
	int somma=0;
	float n=0;
	float media=0;
	int rip=1;
	
	while(rip)
		{
			wait_sem(id_sem,0);
			if(*i==0)
				rip=0;
			else
				{
					printf("Consumatore --> Legge valore =%d \n",*i);
					somma+=*i;
					n++;
				}
			signal_sem(id_sem,0);
		}
	wait_sem(id_sem,1);
	media=somma/n;
	printf("Media =%.2f \n\n",media);
	
	semctl(id_sem,2,IPC_RMID);
	
	shmctl(id_shmem,IPC_RMID,NULL);
}

