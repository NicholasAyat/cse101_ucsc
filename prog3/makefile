CC = gcc
CFLAGS = -Wall -g
OBJS = bpe.o Dictionary.o HashTable.o List.o

all: prog3

prog3: $(OBJS)
	$(CC) $(CFLAGS) -o prog3 $(OBJS)

bpe.o: bpe.c Dictionary.h HashTable.h List.h
Dictionary.o: Dictionary.c Dictionary.h HashTable.h List.h
HashTable.o: HashTable.c HashTable.h
List.o: List.c List.h

clean:
	rm -f *.o prog3
