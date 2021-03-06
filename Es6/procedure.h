#ifndef Procedure_h
#define Procedure_h

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

#define MUTEX 0
#define MUTEX_LIV_SCORTE 1
#define SOSPENSIONE_P 2
#define SOSPENSIONE_C 3

#define MAX_SCORTE 100
#define N_RIP 2
#define N_PROC 20
#define T_SLEEP 2

#define FTOK_PATH "."
#define FTOK_CHAR_SEM 'a'
#define FTOK_CHAR_SHM 'b'


	typedef struct {
		unsigned int id_fornitore;
		unsigned int stato;
		} scaffale;

	int Signal_Sem(int,int);
	int Wait_Sem(int,int);
	int init_sem(int);

	void init_scaffale(scaffale *);

	void produttore(scaffale *,int *,int);
	void cliente(scaffale *,int*,int);

	int produci(scaffale *,int);
	int consuma(scaffale *,int );

#endif
