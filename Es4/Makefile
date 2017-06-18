all: start

start:	main.c procedure
	g++ -o start main.c procedure.o
	./start

procedure:	procedure.c procedure.h
	g++ -c procedure.c

clear :
	rm -f *.o
	rm -f ./start

