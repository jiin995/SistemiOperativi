all:main.c procedure
	gcc -o start main.c procedure.o

procedure:procedure.c procedure.h
	gcc -c procedure.c

clean:
	rm *.o
	rm start
