all: main

main: main.c procedure.h procedure.o monitor.h monitor.o
	gcc -o start main.c procedure.o monitor.o

procedure:procedure.c procedure.h monitor.h
	gcc -c procedure.c
	
monitor: monitor.c monitor.h
	gcc -c monitor.c

clean:
	rm -f *.o
	rm -f start
