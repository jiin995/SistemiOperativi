all: start procedure.o

start: main.c procedure.o header.h monitor.o
	gcc -o start main.c procedure.o monitor.o

procedure.o:procedure.c header.h
	gcc -c procedure.c

monitor.o: monitor.c monitor.h
	gcc -c monitor.c
clean:
	rm *.o
	rm start
