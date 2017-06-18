#include <sys/types.h>
#include <sys/shm.h>
#ifndef lib

#define lib

struct mess{
		int ricevuto=0;
		char msg;
	};

void invio(key_t ,char *);
void ricevi(key_t,char *);

#endif
