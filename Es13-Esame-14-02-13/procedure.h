#ifndef PROCEDURE_H
#define PROCEDURE_H

#define PATH "."
#define ID_SEM 'S'
#define ID 'S'

#define DATA1 "01/02/2016"
#define DATA2 "02/02/2016"
#define DATA3 "03/02/2016"

#define MUTEX 0
#define APPELLO 1
#define PRENOTATI 2

typedef struct {
	char prossimo_appello[20];
	int n_prenotati;
	int n_lettori;
}esame_t;

void inizio_lettura(int ,esame_t *);
void fine_lettura(int,esame_t *);
void inizio_scrittura(int);
void fine_scrittura(int);
void accedi_prenotati(int);
void lascia_prenotati(int);

#endif
