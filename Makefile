CC=gcc
LDFLAGS=
CFLAGS = -Wall -ggdb
EXEC = main
# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.c)

OBJ=$(SRC:.c=.o)

all: $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LDFLAGS)

%.o: %.c %.h typedefs.h
	$(CC) $(CFLAGS) -c -o $@ $<


clean: clear
	rm -f main

clear:
	rm -f *.o
