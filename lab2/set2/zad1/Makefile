CC = gcc
CFLAGS = -Wall
TARGET = main
INPUT = input.txt

all:
	make test

use_lib: $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)_lib -D USE_LIB

use_sys: $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)_sys
test: use_lib use_sys
	echo "Pomiar dla lib" > pomiar_zad_1.txt
	./$(TARGET)_lib A B $(INPUT) output_lib.txt >> pomiar_zad_1.txt
	tr A B < $(INPUT) > tmp
	echo "Wynik tr: " >> pomiar_zad_1.txt
	diff -s tmp output_lib.txt >> pomiar_zad_1.txt
	rm -f tmp

	echo "Pomiar dla sys" >> pomiar_zad_1.txt
	./$(TARGET)_sys A B $(INPUT) output_sys.txt >> pomiar_zad_1.txt
	tr A B < $(INPUT) > tmp
	echo "Wynik tr: " >> pomiar_zad_1.txt
	diff -s tmp output_sys.txt >> pomiar_zad_1.txt
	rm -f tmp


clean:
	rm -f $(TARGET)_lib $(TARGET)_sys output_lib.txt output_sys.txt results_lib.txt results_sys.txt