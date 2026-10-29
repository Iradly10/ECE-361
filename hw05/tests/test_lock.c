
#include <stdio.h>
#include "../lock.h"

/* Keep track of the test results */
static int tests_run = 0;
static int tests_failed = 0;

/* Print PASS or FAIL for each test */
static void check_test(const char *name, int condition)
{
    tests_run++;

    if (condition) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}

/* Test one entry in the transition table */
static void test_transition(state_t current, event_t event,
                            state_t expected_state,
                            action_t expected_action)
{
    action_t action = A_UNDEFINED;
    state_t next = lock_step(current, event, &action);

    char name[100];

    snprintf(name, sizeof(name), "%s + %s",
             state_name(current), event_name(event));

    check_test(name, next == expected_state &&
                     action == expected_action);
}

/* Run a sequence of characters and return the final state */
static state_t run_sequence(const char *input)
{
    state_t state = LOCKED;
    action_t action;

    while (*input != '\0') {
        int event = lock_event(state, *input);

        if (event != -1) {
            state = lock_step(state, (event_t)event, &action);
        }

        input++;
    }

    return state;
}

int main(void)
{
    printf("Testing all 16 transitions\n");

    /* LOCKED transitions */
    test_transition(LOCKED, E_RIGHT, GOT1, A_NONE);
    test_transition(LOCKED, E_WRONG, LOCKED, A_BEEP);
    test_transition(LOCKED, E_CLOSE, LOCKED, A_NONE);
    test_transition(LOCKED, E_TIMEOUT, LOCKED, A_NONE);

    /* GOT1 transitions */
    test_transition(GOT1, E_RIGHT, GOT2, A_NONE);
    test_transition(GOT1, E_WRONG, LOCKED, A_BEEP);
    test_transition(GOT1, E_CLOSE, GOT1, A_NONE);
    test_transition(GOT1, E_TIMEOUT, LOCKED, A_NONE);

    /* GOT2 transitions */
    test_transition(GOT2, E_RIGHT, OPEN, A_UNLOCK);
    test_transition(GOT2, E_WRONG, LOCKED, A_BEEP);
    test_transition(GOT2, E_CLOSE, GOT2, A_NONE);
    test_transition(GOT2, E_TIMEOUT, LOCKED, A_NONE);

    /* OPEN transitions */
    test_transition(OPEN, E_RIGHT, OPEN, A_NONE);
    test_transition(OPEN, E_WRONG, OPEN, A_NONE);
    test_transition(OPEN, E_CLOSE, LOCKED, A_LOCK);
    test_transition(OPEN, E_TIMEOUT, LOCKED, A_LOCK);

    printf("\nTesting lock_event\n");

    /* Correct and incorrect digits in each state */
    check_test("LOCKED correct digit",
               lock_event(LOCKED, '3') == E_RIGHT);
    check_test("LOCKED wrong digit",
               lock_event(LOCKED, '1') == E_WRONG);

    check_test("GOT1 correct digit",
               lock_event(GOT1, '1') == E_RIGHT);
    check_test("GOT1 wrong digit",
               lock_event(GOT1, '3') == E_WRONG);

    check_test("GOT2 correct digit",
               lock_event(GOT2, '4') == E_RIGHT);
    check_test("GOT2 wrong digit",
               lock_event(GOT2, '1') == E_WRONG);

    check_test("OPEN ignores digit",
               lock_event(OPEN, '3') == E_WRONG);

    /* Close, timeout, and invalid characters */
    check_test("Close event",
               lock_event(LOCKED, 'c') == E_CLOSE);
    check_test("Timeout event",
               lock_event(GOT1, 't') == E_TIMEOUT);
    check_test("Invalid character",
               lock_event(LOCKED, 'x') == -1);

    printf("\nTesting combinations\n");

    check_test("314 opens",
               run_sequence("314") == OPEN);

    check_test("3314 stays locked",
               run_sequence("3314") == LOCKED);

    check_test("31t4 stays locked",
               run_sequence("31t4") == LOCKED);

    check_test("314c locks again",
               run_sequence("314c") == LOCKED);

    /* Additional combinations */
    check_test("314t locks again",
               run_sequence("314t") == LOCKED);

    check_test("3c14 opens",
               run_sequence("3c14") == OPEN);

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_run - tests_failed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
