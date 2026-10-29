
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

.PHONY: all clean test

test: $(TARGET)
	@echo "Checking normal washing cycle..."
	@./washer scenarios/normal.txt > /dev/null
	@echo "Checking temperature control..."
	@./washer scenarios/temperature_control.txt > /dev/null
	@echo "Checking midnight rollover..."
	@./washer scenarios/midnight.txt > /dev/null
	@echo "Scenario smoke tests completed successfully."