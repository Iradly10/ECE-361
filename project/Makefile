CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

TARGET = washer

SRCS = main.c sensor.c fsm.c store.c aux.c report.c iom361_r4.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean