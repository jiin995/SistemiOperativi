all: procedure.o
	gcc -lpthread -o start main.c procedure.o

procedure: procedure.h procedure.c
	gcc -c -lpthread procedure.c

clear:
	rm start
	rm *.o
