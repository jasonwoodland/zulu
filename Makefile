TARGET = zulu
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
CC = gcc
CFLAGS = -Wall -g -O0 -std=c99
LDFLAGS = -lm

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

