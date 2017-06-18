all:main studente docente 

main:main.c 
	gcc -o start main.c 

studente:studente.c procedure.o
	gcc -o studente studente.c procedure.o

docente:docente.c procedure.o
	gcc -o docente docente.c procedure.o

procedure.o:procedure.h procedure.c
	gcc -c procedure.c

clean:
	rm *.o
	rm main docente studente
