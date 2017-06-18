all : start

start :      procedure main.c
	gcc -o start main.c procedure.o

procedure :  procedure.c procedure.h
	gcc -c procedure.c

clear : 
	rm -f *.o
	rm -f ./start
