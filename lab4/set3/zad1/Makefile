CC = gcc
CFLAGS = -Wall
TARGET1 = fork
TARGET2 = exec

all:
	make fork_compile

fork_compile:
	$(CC) $(CFLAGS) $(TARGET1).c -o $(TARGET1)

exec_compile:
	$(CC) $(CFLAGS) $(TARGET2).c -o $(TARGET2)

clean:
	rm -f $(TARGET1) $(TARGET2)