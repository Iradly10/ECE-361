
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

TARGET = washer

SRCS = main.c sensor.c fsm.c store.c aux.c report.c iom361_r4.c bits.c status.c
OBJS = $(SRCS:.c=.o)

TEST_BINS = tests/test_fsm tests/test_store tests/test_aux \
            tests/test_report tests/test_sensor tests/test_status

SCENARIOS = normal temperature_control midnight february_rollover \
            door_open stack_growth unexpected_input report

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
	@passed=0; failed=0; \
	for test_bin in $(TEST_BINS); do \
		echo "Running $$test_bin..."; \
		log=$$(mktemp) || exit 1; \
		./$$test_bin > "$$log" 2>&1; \
		rc=$$?; \
		cat "$$log"; \
		p=$$(awk '/^Tests passed:/ { n = $$3 } END { print n+0 }' "$$log"); \
		f=$$(awk '/^Tests failed:/ { n = $$3 } END { print n+0 }' "$$log"); \
		rm -f "$$log"; \
		passed=$$((passed + p)); \
		failed=$$((failed + f)); \
		if [ $$rc -ne 0 ] || [ $$f -ne 0 ] || [ $$p -eq 0 ]; then \
			if [ $$f -eq 0 ]; then failed=$$((failed + 1)); fi; \
		fi; \
	done; \
	for scenario in $(SCENARIOS); do \
		echo "Checking $$scenario scenario..."; \
		actual=$$(mktemp) || exit 1; \
		if [ "$$scenario" = "report" ]; then \
			./washer scenarios/$$scenario.txt --report > "$$actual"; \
		else \
			./washer scenarios/$$scenario.txt > "$$actual"; \
		fi; \
		rc=$$?; \
		if [ $$rc -eq 0 ] && diff -u scenarios/$$scenario.expected "$$actual"; then \
			echo "PASS: $$scenario"; \
			passed=$$((passed + 1)); \
		else \
			echo "FAIL: $$scenario"; \
			failed=$$((failed + 1)); \
		fi; \
		rm -f "$$actual"; \
	done; \
	echo "================================"; \
	echo "Total tests passed: $$passed"; \
	echo "Total tests failed: $$failed"; \
	echo "================================"; \
	if [ $$failed -ne 0 ]; then exit 1; fi

clean:
	rm -f $(TARGET) $(OBJS) $(TEST_BINS)

.PHONY: all clean test
