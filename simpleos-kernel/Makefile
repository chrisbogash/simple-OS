CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = simpleos
OBJS = kernel.o process.o filesystem.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

kernel.o: kernel.c process.h filesystem.h
	$(CC) $(CFLAGS) -c kernel.c

process.o: process.c process.h
	$(CC) $(CFLAGS) -c process.c

filesystem.o: filesystem.c filesystem.h
	$(CC) $(CFLAGS) -c filesystem.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
