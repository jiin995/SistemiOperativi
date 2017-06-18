#ifndef HEADER_H
#define HEADER_H

#define PATH "."

typedef struct{
	long type;
	char msg[80];
}messaggio_t;

void ricevente(int);
void mittente (int,int);

#endif
