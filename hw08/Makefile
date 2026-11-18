CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck bonus clean
all: treestats

treestats: treestats.c bst.c bst.h
	$(CC) $(CFLAGS) treestats.c bst.c -o $@

tests/test_bst: tests/test_bst.c bst.c bst.h
	$(CC) $(CFLAGS) tests/test_bst.c bst.c -o $@

tests/test_remove: tests/test_remove.c bst.c bst.h
	$(CC) $(CFLAGS) tests/test_remove.c bst.c -o $@

test: all tests/test_bst
	./tests/test_bst
	@printf '27 14 35 10 19 31 42\n' | ./treestats | grep -qx 'n 7, height 3'
	@printf '' | ./treestats | grep -qx 'empty tree'
	@printf '1 2 3 nope\n' | ./treestats > tests/.invalid.out; status=$$?; test $$status -eq 1 && grep -qx 'not a number after 3 values' tests/.invalid.out
	@rm -f tests/.invalid.out
	@echo 'PASS treestats smoke tests'

memcheck:
	$(CC) $(CFLAGS) $(SANFLAGS) tests/test_bst.c bst.c -o tests/test_bst_san
	./tests/test_bst_san
	$(CC) $(CFLAGS) $(SANFLAGS) treestats.c bst.c -o treestats_san
	@printf '27 14 35 10 19 31 42\n' | ./treestats_san

bonus:
	$(CC) $(CFLAGS) $(SANFLAGS) tests/test_remove.c bst.c -o tests/test_remove_san
	./tests/test_remove_san

clean:
	rm -f treestats treestats_san tests/test_bst tests/test_bst_san tests/test_remove tests/test_remove_san tests/.invalid.out
