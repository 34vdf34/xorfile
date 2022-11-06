CC = gcc
CFLAGS = -Wall
INCLUDE = -I./include

all: xorfile.o log.o
	$(CC) xorfile.o log.o -o xorfile

xorfile.o: xorfile.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $?

log.o: log.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $?

clean:
	rm -f *.o xorfile
