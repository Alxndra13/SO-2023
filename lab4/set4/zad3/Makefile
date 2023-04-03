CC = gcc
CFLAGS = -Wall
TARGET1 = sender
TARGET2 = catcher

all:
	make sender_compile
	make catcher_compile

sender_compile:
	$(CC) $(CFLAGS) $(TARGET1).c -o $(TARGET1)

catcher_compile:
	$(CC) $(CFLAGS) $(TARGET2).c -o $(TARGET2)

clean:
	rm -f $(TARGET1) $(TARGET2)