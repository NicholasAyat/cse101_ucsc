manager: formatter.c List.c List.h Stack.c Stack.h
	cc -o prog1 -g formatter.c List.c Stack.c

%.o: %.c
	cc -c -o $@ $< -std=c99

clean:
	rm prog1
