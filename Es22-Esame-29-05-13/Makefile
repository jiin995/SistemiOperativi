all: server start client

start:server client header.h main.c
	gcc -o start main.c

server:server.c header.h
	gcc -lpthread -o server server.c

client:client.c header.h
	gcc -o client client.c

clean:
	rm *.o
	rm server client start
