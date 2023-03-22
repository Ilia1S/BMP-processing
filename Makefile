CC = clang
CFLAGS = -Wall -Werror
ARGS = arg1 arg2 arg3

all: main
	./main $(ARGS)

main: main.o BMPprocessing.o
	$(CC) $(CFLAGS) -o main main.o BMPprocessing.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

BMPprocessing.o: BMPprocessing.c
	$(CC) $(CFLAGS) -c BMPprocessing.c

clean:
	rm -f main *.o
