
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

TARGET = washer

SRCS = main.c sensor.c fsm.c store.c aux.c report.c iom361_r4.c bits.c
OBJS = $(SRCS:.c=.o)

TEST_BINS = tests/test_fsm tests/test_store tests/test_aux \
            tests/test_report tests/test_sensor

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests/test_fsm: tests/test_fsm.c fsm.c fsm.h
	$(CC) $(CFLAGS) -I. tests/test_fsm.c fsm.c -o $@

tests/test_store: tests/test_store.c store.c store.h
	$(CC) $(CFLAGS) -I. tests/test_store.c store.c -o $@

tests/test_aux: tests/test_aux.c aux.c aux.h
	$(CC) $(CFLAGS) -I. tests/test_aux.c aux.c -o $@

tests/test_report: tests/test_report.c report.c store.c report.h store.h
	$(CC) $(CFLAGS) -I. tests/test_report.c report.c store.c -o $@

tests/test_sensor: tests/test_sensor.c sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -I. tests/test_sensor.c sensor.c -lm -o $@

test: $(TARGET) $(TEST_BINS)
	@echo "Running FSM unit tests..."
	@./tests/test_fsm
	@echo "Running BST unit tests..."
	@./tests/test_store
	@echo "Running event stack unit tests..."
	@./tests/test_aux
	@echo "Running report unit tests..."
	@./tests/test_report
	@echo "Running sensor unit tests..."
	@./tests/test_sensor
	@echo "Checking normal washing cycle..."
	@./washer scenarios/normal.txt > /dev/null
	@echo "Checking temperature control..."
	@./washer scenarios/temperature_control.txt > /dev/null
	@echo "Checking midnight rollover..."
	@./washer scenarios/midnight.txt > /dev/null
	@echo "Checking February rollover..."
	@./washer scenarios/february_rollover.txt | grep -q "2027-03-01 00:00:00"
	@echo "Checking door-open scenario..."
	@./washer scenarios/door_open.txt > /dev/null
	@echo "Checking stack growth..."
	@./washer scenarios/stack_growth.txt > /dev/null
	@echo "Checking unexpected input..."
	@./washer scenarios/unexpected_input.txt > /dev/null
	@echo "Checking report against expected output..."
	@./washer scenarios/report.txt --report | diff -u scenarios/report.expected -
	@echo "All configured tests completed successfully."

clean:
	rm -f $(TARGET) $(OBJS) $(TEST_BINS)

.PHONY: all clean test
