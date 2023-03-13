CC = gcc
CFLAGS = -Wall
LIB_NAME = libwc
TARGET = repl

all:
	make exec_static
	make exec_shared

exec_static: static
	$(CC) $(CFLAGS) $(TARGET).c $(LIB_NAME).a -o $(TARGET)_static

exec_shared: shared
	$(CC) $(CFLAGS) $(TARGET).c $(LIB_NAME).so -o $(TARGET)_shared

static: $(LIB_NAME).c
	$(CC) $(CFLAGS) -c $(LIB_NAME).c
	ar cr $(LIB_NAME).a $(LIB_NAME).o

shared: $(LIB_NAME).c
	$(CC) $(CFLAGS) -c -fPIC $(LIB_NAME).c
	$(CC) $(CFLAGS) -shared -o $(LIB_NAME).so $(LIB_NAME).o

clean:
	rm -f *.o *.a *.so $(TARGET)_static $(TARGET)_shared