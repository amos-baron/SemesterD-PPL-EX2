CC = gcc
CFLAGS = -m32 -g -Wall

all: myshell looper Printers mypipe

myshell: myshell.o LineParser.o
	$(CC) $(CFLAGS) myshell.o LineParser.o -o myshell

looper: looper.o
	$(CC) $(CFLAGS) looper.o -o looper

Printers: Printers.o
	$(CC) $(CFLAGS) Printers.o -o Printers

mypipe: mypipe.o
	$(CC) $(CFLAGS) mypipe.o -o mypipe

myshell.o: myshell.c LineParser.h
	$(CC) $(CFLAGS) -c myshell.c -o myshell.o

LineParser.o: LineParser.c LineParser.h
	$(CC) $(CFLAGS) -c LineParser.c -o LineParser.o

looper.o: looper.c
	$(CC) $(CFLAGS) -c looper.c -o looper.o

Printers.o: Printers.c
	$(CC) $(CFLAGS) -c Printers.c -o Printers.o

mypipe.o: mypipe.c
	$(CC) $(CFLAGS) -c mypipe.c -o mypipe.o

clean:
	rm -f myshell looper Printers mypipe *.o

.PHONY: all clean