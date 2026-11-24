CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g
TARGET = tests/test_ht
SAN_TARGET = tests/test_ht_san
.PHONY: all test memcheck clean
all: $(TARGET)
$(TARGET): ht.c ht.h tests/test_ht.c
	$(CC) $(CFLAGS) -I. ht.c tests/test_ht.c -o $@
test: $(TARGET)
	./$(TARGET)
memcheck: $(TARGET)
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --error-exitcode=1 ./$(TARGET); \
	else \
		$(CC) $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer -I. ht.c tests/test_ht.c -o $(SAN_TARGET) && \
		if [ "$$(uname -s)" = Darwin ]; then \
			ASAN_OPTIONS=detect_leaks=0 ./$(SAN_TARGET) && leaks --atExit -- ./$(TARGET); \
		else ASAN_OPTIONS=detect_leaks=1 ./$(SAN_TARGET); fi; \
	fi
clean:
	rm -f $(TARGET) $(SAN_TARGET)
