
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
LDLIBS = -lm

OBJS = main.o sensor.o panel.o scenario.o iom361_r4.o

.PHONY: all test clean

all: replay

replay: $(OBJS)
	$(CC) $(CFLAGS) -o replay $(OBJS) $(LDLIBS)

main.o: main.c sensor.h panel.h scenario.h iom361_r4.h
	$(CC) $(CFLAGS) -c main.c

sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c sensor.c

panel.o: panel.c panel.h iom361_r4.h
	$(CC) $(CFLAGS) -c panel.c

scenario.o: scenario.c scenario.h
	$(CC) $(CFLAGS) -c scenario.c

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c iom361_r4.c

test: replay
	@echo "Replay program built successfully."
	@echo "Unit and regression tests will be added next."

clean:
	rm -f replay *.o tests/*.o tests/test_hw06
