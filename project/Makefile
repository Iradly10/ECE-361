
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

TARGET = washer

SRCS = main.c sensor.c fsm.c store.c aux.c report.c iom361_r4.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
