#ifndef procedure
#define procedure

#define INBOX_SEND 's'
#define INBOX_RECIVE 'r'
#define PATH "."
#define N_CLIENT 2

#include <pthread.h>

pthread_mutex_t mutex_Sendbox;


typedef struct {
	long type;
	int n1;
	int n2;
}msgs;

typedef struct {
	long type;
	int result;
}msgr;

typedef struct {
	msgs message;
	int send_Inbox;
	}input_Struct;

int Server_Main(int,int);
int Client(int,int);
void* Server_Thread(void *);

#endif
