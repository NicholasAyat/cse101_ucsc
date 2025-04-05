CC = gcc
CFLAGS = -Wall -I/usr/include/cairo
LDFLAGS = -lcairo

# If using local Cairo installation
ifdef CAIRO_LOCAL
    CFLAGS += -I./cairo/include
    LDFLAGS = -L./cairo/lib -lcairo
endif

TARGET = hwk0
SRCS = program.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) output.png 