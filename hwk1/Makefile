hwk1: hwk1.o List.o List.h Stack.o Stack.h
	cc -o hwk1 hwk1.o List.o Stack.o

%.o: %.c
	cc -c -o $@ $< -std=c99

clean:
	rm -f hwk1 *.o
