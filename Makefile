CC = gcc
CFLAGS = -m32 -g -Wall
HEADERS = $(wildcard *.h)
SOURCES = $(wildcard *.c)

PROGRAMS = myshell

all: $(PROGRAMS)

myshell: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o myshell

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean