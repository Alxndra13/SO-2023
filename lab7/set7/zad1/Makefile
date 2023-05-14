CC = gcc
CFLAGS = -Wall

all:
	make compile

compile:
	$(CC) $(CFLAGS) client.c common.c -lrt -lpthread -o client
	$(CC) $(CFLAGS) hairdresser.c common.c -lrt -lpthread -o hairdresser
	$(CC) $(CFLAGS) simulation.c common.c -lrt -lpthread -o simulation

	$(CC) $(CFLAGS) simulation.c -o simulation

clean:
	rm -f common client hairdresser simulation