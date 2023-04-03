CC = gcc
CFLAGS = -Wall
TARGET1 = fork
TARGET2 = exec

all:
	make fork_compile
	make exec_compile

fork_compile:
	$(CC) $(CFLAGS) $(TARGET1).c -o $(TARGET1)

exec_compile:
	$(CC) $(CFLAGS) $(TARGET2).c -o $(TARGET2)

raport: fork_compile exec_compile
	echo "ignore" > $(TARGET1)_result.txt
	./$(TARGET1) ignore >> $(TARGET1)_result.txt

	echo "handler" >> $(TARGET1)_result.txt
	./$(TARGET1) handler >> $(TARGET1)_result.txt

	echo "mask" >> $(TARGET1)_result.txt
	./$(TARGET1) mask >> $(TARGET1)_result.txt

	echo "pending" >> $(TARGET1)_result.txt
	./$(TARGET1) pending >> $(TARGET1)_result.txt


	echo "ignore" > $(TARGET2)_result.txt
	./$(TARGET2) ignore >> $(TARGET2)_result.txt

	echo "mask" >> $(TARGET2)_result.txt
	./$(TARGET2) mask >> $(TARGET2)_result.txt

	echo "pending" >> $(TARGET2)_result.txt
	./$(TARGET2) pending >> $(TARGET2)_result.txt


clean:
	rm -f $(TARGET1) $(TARGET2) *_result.txt