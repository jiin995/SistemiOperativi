#ifndef HEADER_H
#define HEADER_H

#define FTOK_PATH 	"."
#define FTOK_CHAR_RIC	'a'
#define FTOK_CHAR_RIS	'b'

typedef struct{

	long type;
	
	int v1;
	int v2;
}msg_richiesta;

typedef struct{
	
	long type;
	
	int ris;
}msg_risposta;

#endif
