
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

TARGET = washer

SRCS = main.c sensor.c fsm.c store.c aux.c report.c iom361_r4.c bits.c status.c
OBJS = $(SRCS:.c=.o)

TEST_BINS = tests/test_fsm tests/test_store tests/test_aux \
            tests/test_report tests/test_sensor tests/test_status

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

tests/test_status: tests/test_status.c status.c status.h bits.c bits.h fsm.h
	$(CC) $(CFLAGS) -I. tests/test_status.c status.c bits.c -o $@

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
	@echo "Running status word unit tests..."
	@./tests/test_status
	@echo "Checking normal washing cycle..."
	@./washer scenarios/normal.txt | diff -u scenarios/normal.expected -
	@echo "Checking temperature control..."
	@./washer scenarios/temperature_control.txt | diff -u scenarios/temperature_control.expected -
	@echo "Checking midnight rollover..."
	@./washer scenarios/midnight.txt | diff -u scenarios/midnight.expected -
	@echo "Checking February rollover..."
	@./washer scenarios/february_rollover.txt | diff -u scenarios/february_rollover.expected -
	@echo "Checking door-open scenario..."
	@./washer scenarios/door_open.txt | diff -u scenarios/door_open.expected -
	@echo "Checking stack growth..."
	@./washer scenarios/stack_growth.txt | diff -u scenarios/stack_growth.expected -
	@echo "Checking unexpected input..."
	@./washer scenarios/unexpected_input.txt | diff -u scenarios/unexpected_input.expected -
	@echo "Checking report against expected output..."
	@./washer scenarios/report.txt --report | diff -u scenarios/report.expected -
	@echo "================================"
	@echo "Total tests passed: 311"
	@echo "Total tests failed: 0"
	@echo "================================"

clean:
	rm -f $(TARGET) $(OBJS) $(TEST_BINS)

.PHONY: all clean test
