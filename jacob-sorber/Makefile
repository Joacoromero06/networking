CC = gcc
CFLAGS =  -Wall

all: tcpc websv

tcpc: tcp-client.c
	$(CC) $(CFLAGS) -o tcpc tcp-client.c

websv: web-server.c
	$(CC) $(CFLAGS) -o websv web-server.c

clean:
	rm -rf *.dSYM tcpc
