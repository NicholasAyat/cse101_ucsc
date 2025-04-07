CC = gcc
CFLAGS = -Wall -g `pkg-config --cflags cairo`
LDFLAGS = `pkg-config --libs cairo` -lm

all: prog0 bonus

prog0: prog0.c
	$(CC) $(CFLAGS) -o prog0 prog0.c $(LDFLAGS)

bonus: bonus.c
	$(CC) $(CFLAGS) -o bonus bonus.c $(LDFLAGS)

clean:
	rm -f prog0 bonus square.png bonus.png 