CC = gcc
CFLAGS = -Wall
LIB_NAME = libwc

all:
	make static
	make shared

static: $(LIB_NAME).c
	$(CC) $(CFLAGS) -c $(LIB_NAME).c
	ar cr $(LIB_NAME).a $(LIB_NAME).o

shared: $(LIB_NAME).c
	$(CC) $(CFLAGS) -fPIC -c $(LIB_NAME).c
	$(CC) $(CFLAGS) -shared -o $(LIB_NAME).so $(LIB_NAME).o

clean:
	rm -f $(LIB_NAME).o $(LIB_NAME).a $(LIB_NAME).so

.PHONY: all static shared clean