all: start procedure.o

start:procedure.o header.h main.c
	gcc -lpthread -o start main.c procedure.o

procedure.o:procedure.c header.h
	gcc -c procedure.c

clean:
	rm *.o
	rm start
