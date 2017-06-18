all: main.c procedure.o monitor.o
	gcc -o start main.c procedure.o monitor.o

procedure.o: procedure.c 
	gcc -c procedure.c 

monitor.o: monitor.c
	gcc -c monitor.c

clear:
	rm *.o
	rm start
