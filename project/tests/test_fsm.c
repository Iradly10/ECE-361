
#include <stdio.h>
#include <string.h>
#include "../fsm.h"

static int tests_run = 0;
static int tests_failed = 0;

#define CHECK(condition) do {                         \
    tests_run++;                                      \
    if (condition) {                                  \
        printf("PASS: %s\n", #condition);             \
    } else {                                          \
        printf("FAIL: %s (line %d)\n",                \
               #condition, __LINE__);                 \
        tests_failed++;                               \
    }                                                 \
} while (0)

int main(void)
{
    printf("Testing Washing Machine FSM\n\n");

    /* Test normal washing sequence */
    CHECK(fsm_next_state(STATE_IDLE, EVENT_START)
          == STATE_FILL);

    CHECK(fsm_next_state(STATE_FILL, EVENT_FILL_COMPLETE)
          == STATE_WASH);

    CHECK(fsm_next_state(STATE_WASH, EVENT_WASH_COMPLETE)
          == STATE_RINSE);

    CHECK(fsm_next_state(STATE_RINSE, EVENT_RINSE_COMPLETE)
          == STATE_SPIN);

    CHECK(fsm_next_state(STATE_SPIN, EVENT_SPIN_COMPLETE)
          == STATE_DRAIN);

    CHECK(fsm_next_state(STATE_DRAIN, EVENT_DRAIN_COMPLETE)
          == STATE_IDLE);

    /* Test door-open safety behavior */
    CHECK(fsm_next_state(STATE_FILL, EVENT_DOOR_OPEN)
          == STATE_IDLE);

    CHECK(fsm_next_state(STATE_WASH, EVENT_DOOR_OPEN)
          == STATE_IDLE);

    CHECK(fsm_next_state(STATE_RINSE, EVENT_DOOR_OPEN)
          == STATE_IDLE);

    CHECK(fsm_next_state(STATE_SPIN, EVENT_DOOR_OPEN)
          == STATE_IDLE);

    CHECK(fsm_next_state(STATE_DRAIN, EVENT_DOOR_OPEN)
          == STATE_IDLE);

    /* Test ignored events */
    CHECK(fsm_next_state(STATE_IDLE, EVENT_DOOR_OPEN)
          == STATE_IDLE);

    CHECK(fsm_next_state(STATE_WASH, EVENT_START)
          == STATE_WASH);

    CHECK(fsm_next_state(STATE_IDLE, EVENT_WASH_COMPLETE)
          == STATE_IDLE);

    /* Test transition actions */
    CHECK(fsm_get_transition(STATE_IDLE, EVENT_START).action
          == ACTION_START_FILL);

    CHECK(fsm_get_transition(STATE_FILL, EVENT_DOOR_OPEN).action
          == ACTION_STOP);

    CHECK(fsm_get_transition(STATE_DRAIN, EVENT_DRAIN_COMPLETE).action
          == ACTION_FINISH);

    /* Test state names */
    CHECK(strcmp(fsm_state_name(STATE_IDLE), "IDLE") == 0);
    CHECK(strcmp(fsm_state_name(STATE_WASH), "WASH") == 0);
    CHECK(strcmp(fsm_state_name(STATE_DRAIN), "DRAIN") == 0);

    /* Test invalid inputs */
    CHECK(fsm_next_state(STATE_COUNT, EVENT_START)
          == STATE_COUNT);

    CHECK(fsm_next_state(STATE_IDLE, EVENT_COUNT)
          == STATE_IDLE);

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_run - tests_failed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
