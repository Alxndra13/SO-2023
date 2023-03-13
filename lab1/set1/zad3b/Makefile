SHELL = /bin/bash
CC = gcc
CFLAGS = -Wall
LIB_NAME = libwc
TARGET = repl

all:
	make exec_static
	make exec_shared
	make report_3b

optimized_test:
	make test OPT_TYPE="-O"
	make test OPT_TYPE="-O0"
	make test OPT_TYPE="-O1"
	make test OPT_TYPE="-O2"
	make test OPT_TYPE="-O3"
	make test OPT_TYPE="-Os"

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

test: exec_static exec_shared create_test
	mkdir -p results/
	./$(TARGET)_shared <test_input.txt> results/results_shared_$(OPT_TYPE).txt
	./$(TARGET)_static <test_input.txt> results/results_static_$(OPT_TYPE).txt

create_test:
	echo "init 100" > test_input.txt
	echo "count $(TARGET).c" >> test_input.txt
	echo "count Makefile" >> test_input.txt
	echo "count $(LIB_NAME).c" >> test_input.txt
	echo "count $(LIB_NAME).h" >> test_input.txt
	echo "show 0" >> test_input.txt
	echo "show 1" >> test_input.txt
	echo "show 2" >> test_input.txt
	echo "show 3" >> test_input.txt
	echo "delete 0" >> test_input.txt
	echo "delete 1" >> test_input.txt
	echo "delete 2" >> test_input.txt
	echo "delete 3" >> test_input.txt
	echo "destroy" >> test_input.txt
	echo "quit" >> test_input.txt

report_3a: test
	echo "" > report.txt
	./script.sh

report_3b: optimized_test
	echo "" > report.txt
	./script.sh

clean:
	rm -f *.o *.a *.so $(TARGET)_static $(TARGET)_shared test_input.txt results/results*.txt
	rmdir results