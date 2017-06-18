all: main.c procedure.o
	gcc -lpthread -o start main.c procedure.o

procedure: procedure.c
	gcc -c procedure.c

clear:
	rm *.o
	rm start
