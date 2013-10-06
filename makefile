#PROGRAM: Nome do executavel
#CC: Compilador
#CFLAGS: Flags de compilação
PROGRAM=main
CC=mpicc
CFLAGS=-c -Wall -lm

all: program

program: main.o libsort.o
	$(CC) main.o libsort.o -o $(PROGRAM)

main.o: main.c
	$(CC) $(CFLAGS) main.c timer.h

libsort.o: libsort.c libsort.h
	$(CC) $(CFLAGS) libsort.c

clean:
	rm -rf *o

uninstall:
	rm -rf $(PROGRAM)