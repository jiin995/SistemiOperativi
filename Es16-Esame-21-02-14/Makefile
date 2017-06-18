all: start procedure

start:main.c procedure
	gcc -lpthread -o start main.c procedure.o

procedure:procedure.c procedure.h
	gcc -c procedure.c

clean:
	rm *.o
	rm start
