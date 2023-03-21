CC = gcc
CFLAGS = -Wall
TARGET = main

all:
	make run

compile:
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

run: compile
	./$(TARGET)
	wc --bytes *
clean:
	rm -f $(TARGET)