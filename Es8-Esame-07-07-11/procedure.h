/* Procedure.h */

#ifndef procedure
#define procedure

#define N_THREAD 5
#define N_PUSH 4
#define N_POP (N_THREAD*N_PUSH)/2


#include <pthread.h>

typedef int Elem;

typedef struct Stack {
	Elem *dati;
	int dim;
	int size;
	pthread_mutex_t mutex;		//Inizialmente unlocked
	pthread_cond_t ok_pop;		//Inizialmente unlocked && ok_pop <=> size!=0 
	pthread_cond_t ok_push;		//Inizialmente unlocked && ok_push <=> size<dim
}Stack;

void StackInit (Stack *,int);
void StackRemove(Stack *);
void StackPush(Stack *,Elem);
Elem StackPop(Stack *);
int  StackSize(Stack *);

#endif
