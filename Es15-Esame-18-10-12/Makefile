all:start p1 p2 p3 p4 p5 p6
	
start:main.c header.h
	gcc -o start main.c

p1:header.h p1.c
	gcc -o p1 p1.c
p2:header.h p2.c
	gcc -o p2 p2.c
p3:header.h p3.c
	gcc -o p3 p3.c
p4:header.h p4.c
	gcc -o p4 p4.c
p5:header.h p5.c
	gcc -o p5 p5.c
p6:header.h p6.c
	gcc -o p6 p6.c

clean:
	rm *.o
	rm p*
	rm start
