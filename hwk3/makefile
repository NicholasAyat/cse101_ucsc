CC = gcc
CFLAGS = -Wall -g
OBJS = hwk3.o Dictionary.o HashTable.o List.o

all: hwk3

hwk3: $(OBJS)
	$(CC) $(CFLAGS) -o hwk3 $(OBJS)

hwk3.o: hwk3.c Dictionary.h HashTable.h List.h
Dictionary.o: Dictionary.c Dictionary.h HashTable.h List.h
HashTable.o: HashTable.c HashTable.h
List.o: List.c List.h

clean:
	rm -f *.o hwk3
