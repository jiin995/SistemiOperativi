#ifndef HEADER_H
#define HEADER_H

#define PATH "."
#define SERVER_QUEUE 'a'
#define PRINTER_QUEUE 'b' 

#define N_CLIENT 5
#define N_RICHIESTE 15
#define N_BUF 10


typedef struct{
	long type;
	int pid;

}message_t;

void Client  (int);
void Printer (int);
void Server  (int,int);

int Inserisci (int,message_t *);
int Preleva   (int,message_t *);

#endif
