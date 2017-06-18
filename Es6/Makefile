all: main.o procedure.o
	
main.o:	main.c	procedure.o
	gcc -o start main.c procedure.o

procedure.o: procedure.h procedure.c
	gcc -c procedure.c

clear:
	rm -f *.o
	rm -f start
