
# ECE 361 - Homework 6 Makefile

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I.
LDLIBS = -lm

# Main program object files
OBJS = main.o sensor.o panel.o scenario.o iom361_r4.o

# Unit test files
TEST_EXE = tests/test_hw06
TEST_SRCS = tests/test_hw06.c sensor.c panel.c scenario.c iom361_r4.c

.PHONY: all test clean

# Build the replay program
all: replay

replay: $(OBJS)
	$(CC) $(CFLAGS) -o replay $(OBJS) $(LDLIBS)

# Compile main program
main.o: main.c sensor.h panel.h scenario.h iom361_r4.h
	$(CC) $(CFLAGS) -c main.c

# Compile sensor module
sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c sensor.c

# Compile panel module
panel.o: panel.c panel.h iom361_r4.h
	$(CC) $(CFLAGS) -c panel.c

# Compile scenario module
scenario.o: scenario.c scenario.h
	$(CC) $(CFLAGS) -c scenario.c

# Compile I/O emulator
iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c iom361_r4.c

# Build unit test executable
$(TEST_EXE): $(TEST_SRCS) sensor.h panel.h scenario.h iom361_r4.h
	$(CC) $(CFLAGS) -o $(TEST_EXE) $(TEST_SRCS) $(LDLIBS)

# Run unit tests and regression tests
test: replay $(TEST_EXE)
	./$(TEST_EXE)
	sh regress.sh

# Remove compiled files
clean:
	rm -f replay *.o tests/*.o $(TEST_EXE)
