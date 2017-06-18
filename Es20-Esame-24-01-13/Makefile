all:start procedure.o

start: main.c procedure.o
	gcc -lpthread -o start main.c procedure.o

procedure.o: procedure.c procedure.h
	gcc -c procedure.c

clean: 
	rm *.o
	rm start
