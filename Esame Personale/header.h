#ifndef HEADER_H
#define HEADER_H

#define STATO_MAGAZZINO 0
#define ACQUISTO 	1
#define FORNITURA	2


#define DIM_MAG 10

typedef struct{
	
	int costo;
	int quantita;
	
}Magazzino;

typedef struct{

	int numero;
	int quantita;

	int costo;
}articolo;

typedef struct{
	
	long type;

	int tipo_richiesta;
	
	articolo art;

}msg_richiesta;

typedef struct{
	
	long type;

	int tipo_risposta;

	Magazzino mag[DIM_MAG];

	articolo art;
}msg_risposta;

void cliente(int ,int);
void gestore(int ,int);
void fornitori(int);


#endif