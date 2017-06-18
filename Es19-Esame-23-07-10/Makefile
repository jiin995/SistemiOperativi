all:start procedure.o

start: main.c header.h procedure.o
	gcc -o start main.c procedure.o

procedure.o:procedure.c header.h
	gcc -c procedure.c

clean: 
	rm *.o
	rm start
