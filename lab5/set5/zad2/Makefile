CC = gcc
CFLAGS = -Wall
TARGET = main
ACCURACY = 0.000000001

all:
	make compile
	make report

compile:
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

report: compile
	echo "Pomiary dla dokładności $(ACCURACY)" > report.txt
	echo "n = 1" >> report.txt
	./$(TARGET) $(ACCURACY) 1 >> report.txt
	echo "" >> report.txt
	echo "n = 10" >> report.txt
	./$(TARGET) $(ACCURACY) 10 >> report.txt
	echo "" >> report.txt
	echo "n = 100" >> report.txt
	./$(TARGET) $(ACCURACY) 100 >> report.txt
	echo "" >> report.txt
	echo "n = 1000" >> report.txt
	./$(TARGET) $(ACCURACY) 1000 >> report.txt
	echo "" >> report.txt
	echo "n = 10000" >> report.txt
	./$(TARGET) $(ACCURACY) 10000 >> report.txt
clean:
	rm -f $(TARGET)