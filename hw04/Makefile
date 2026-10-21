CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

all: events

events: events.o tally.o
	$(CC) $(CFLAGS) events.o tally.o -o events

events.o: events.c tally.h
	$(CC) $(CFLAGS) -c events.c

tally.o: tally.c tally.h
	$(CC) $(CFLAGS) -c tally.c

clean:
	rm -f events events.o tally.o