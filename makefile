compile: server.c client.c headers.h
	gcc -o srvr server.c
	gcc -o clnt client.c

server: srvr
	./srvr

client: clnt
	./clnt

srvr: server.c headers.h
	gcc -o srvr server.c

clnt: client.c headers.h
	gcc -o clnt client.c
