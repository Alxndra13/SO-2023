CC = gcc
CFLAGS = -Wall

all:
	make compile

compile:
	$(CC) $(CFLAGS) client.c common.c -o client
	$(CC) $(CFLAGS) hairdresser.c common.c -o hairdresser
	$(CC) $(CFLAGS) simulation.c common.c -o simulation

clean:
	rm -f common client hairdresser simulation