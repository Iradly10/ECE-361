
CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck clean

all: balance

balance: balance.c istack.c istack.h
	$(CC) $(CFLAGS) -o $@ balance.c istack.c

tests/test_istack: tests/test_istack.c istack.c istack.h
	$(CC) $(CFLAGS) -o $@ tests/test_istack.c istack.c

test: balance tests/test_istack
	./tests/test_istack
	sh ./regress.sh

memcheck:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) $(SANFLAGS)" all tests/test_istack
	ASAN_OPTIONS=detect_leaks=0 ./tests/test_istack
	ASAN_OPTIONS=detect_leaks=0 sh ./regress.sh

clean:
	rm -f balance tests/test_istack
