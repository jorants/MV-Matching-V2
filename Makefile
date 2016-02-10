CC=gcc
LDFLAGS=-lc --entry main
CFLAGS = -Wall
EXEC = main
# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(OBJ)
	ld -o $(EXEC) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean: clear
	rm -f main

clear:
	rm -f *.o
