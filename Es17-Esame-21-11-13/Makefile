all: chat mittente ricevente 

chat:main.c procedure.o
	gcc -o chat main.c procedure.c

mittente:mittente.c header.h
	gcc -o mittente mittente.c

ricevente:ricevente.c header.h
	gcc -o ricevente ricevente.c

procedure.o:header.h procedure.c
	gcc -c procedure.c

clean:
	rm *.o
	rm mittente ricevente chat
