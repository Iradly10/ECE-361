
#include <stdio.h>
#include <stdint.h>
#include "bits.h"
#include "fsm.h"

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define CHECK(condition) do {                       \
    tests_run++;                                    \
    if (condition) {                                \
        tests_passed++;                             \
        printf("PASS: %s\n", #condition);           \
    } else {                                        \
        tests_failed++;                             \
        printf("FAIL: %s\n", #condition);           \
    }                                               \
} while (0)

static uint32_t pack_status(washer_state_t state,
                            uint32_t leds)
{
    uint32_t status = 0;

    status = set_field(status, 0, 3, (uint32_t)state);
    status = set_field(status, 3, 4, leds);
    status = set_field(status, 7, 1,
                       state != STATE_IDLE ? 1u : 0u);

    return status;
}

static void test_status_fields(void)
{
    uint32_t status = pack_status(STATE_WASH, 0x6u);

    CHECK(get_field(status, 0, 3) == (uint32_t)STATE_WASH);
    CHECK(get_field(status, 3, 4) == 0x6u);
    CHECK(get_field(status, 7, 1) == 1u);
    CHECK(get_field(status, 8, 24) == 0u);
}

static void test_idle_status(void)
{
    uint32_t status = pack_status(STATE_IDLE, 0u);

    CHECK(get_field(status, 0, 3) == (uint32_t)STATE_IDLE);
    CHECK(get_field(status, 3, 4) == 0u);
    CHECK(get_field(status, 7, 1) == 0u);
    CHECK(get_field(status, 8, 24) == 0u);
}

static void test_all_states(void)
{
    for (int state = STATE_IDLE; state < STATE_COUNT; state++)
    {
        uint32_t status =
            pack_status((washer_state_t)state, 0xFu);

        CHECK(get_field(status, 0, 3) == (uint32_t)state);
        CHECK(get_field(status, 3, 4) == 0xFu);
        CHECK(get_field(status, 7, 1) ==
              (state == STATE_IDLE ? 0u : 1u));
        CHECK(get_field(status, 8, 24) == 0u);
    }
}

static void test_led_patterns(void)
{
    for (uint32_t leds = 0; leds <= 0xFu; leds++)
    {
        uint32_t status = pack_status(STATE_FILL, leds);

        CHECK(get_field(status, 3, 4) == leds);
    }
}

int main(void)
{
    printf("Testing Washing Machine Status Word\n\n");

    test_status_fields();
    test_idle_status();
    test_all_states();
    test_led_patterns();

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
