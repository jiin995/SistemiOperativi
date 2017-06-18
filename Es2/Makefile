all: start

start: lib.o main.c
	g++ -o start main.c lib.o

lib.o: lib.c
	g++ -c lib.c

clear:
	rm -f *.o
	rm -f ./start
	rm -f *~
