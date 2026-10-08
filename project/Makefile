CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

TARGET = washer

SRCS = main.c sensor.c fsm.c store.c aux.c report.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean