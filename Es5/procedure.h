#ifndef PROCEDURE_H 
#define PROCEDURE_H


#include <sys/types.h>

#define FTOK_PATH "."
#define FTOK_SHM_CHAR 'a'
#define FTOK_SEM_CHAR 'b'
#define BUFFER_DIM 5
#define NSEM 4
#define MUTEXP 0
#define MUTEXC 1
#define MSG_DISP 2
#define SPAZIO_DISP 3
#define IN_USO 2
#define PIENO 1
#define LIBERO 0

typedef pid_t msg; 

typedef  struct {
            msg val[BUFFER_DIM];
            int stato[BUFFER_DIM];
        } buffer ;

void buffer_init(buffer *);
void produttore(buffer *buf,int id_sem);
void consumatore(buffer *buf,int id_sem);
void Wait_Sem(int ,int);
void Signal_Sem(int,int);
msg produci();
int consuma(msg);
void sem_init(int);

#endif