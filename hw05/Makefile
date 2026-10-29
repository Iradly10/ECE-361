
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

.PHONY: all test clean

all: lock

lock: main.o lock.o
	$(CC) $(CFLAGS) -o lock main.o lock.o

main.o: main.c lock.h
	$(CC) $(CFLAGS) -c main.c

lock.o: lock.c lock.h
	$(CC) $(CFLAGS) -c lock.c

tests/test_lock.o: tests/test_lock.c lock.h
	$(CC) $(CFLAGS) -c tests/test_lock.c -o tests/test_lock.o

test_lock: tests/test_lock.o lock.o
	$(CC) $(CFLAGS) -o test_lock tests/test_lock.o lock.o

test: test_lock
	./test_lock

clean:
	rm -f lock test_lock main.o lock.o tests/test_lock.o
