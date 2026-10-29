
#include <stdio.h>
#include "../aux.h"

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
    printf("Testing Washing Machine Event Stack\n\n");

    /* Create an empty stack. */
    aux_t *stack = aux_create();

    CHECK(stack != NULL);

    if (stack == NULL)
    {
        return 1;
    }

    /* Test empty stack behavior. */
    CHECK(aux_count(stack) == 0);
    CHECK(aux_is_empty(stack) != 0);

    aux_event_t event = {0};

    CHECK(aux_pop(stack, &event) != 0);
    CHECK(aux_peek(stack, &event) != 0);

    /* Add the first event. */
    aux_event_t first = {1, STATE_FILL};

    CHECK(aux_push(stack, first) == 0);
    CHECK(aux_count(stack) == 1);
    CHECK(aux_is_empty(stack) == 0);

    /* Check the top event without removing it. */
    CHECK(aux_peek(stack, &event) == 0);
    CHECK(event.tick == 1);
    CHECK(event.state == STATE_FILL);
    CHECK(aux_count(stack) == 1);

    /* Add a second event. */
    aux_event_t second = {21, STATE_WASH};

    CHECK(aux_push(stack, second) == 0);
    CHECK(aux_count(stack) == 2);

    /* The most recent event should be on top. */
    CHECK(aux_peek(stack, &event) == 0);
    CHECK(event.tick == 21);
    CHECK(event.state == STATE_WASH);

    /* Remove the most recent event first. */
    CHECK(aux_pop(stack, &event) == 0);
    CHECK(event.tick == 21);
    CHECK(event.state == STATE_WASH);

    /* The first event should now be on top. */
    CHECK(aux_pop(stack, &event) == 0);
    CHECK(event.tick == 1);
    CHECK(event.state == STATE_FILL);

    CHECK(aux_count(stack) == 0);
    CHECK(aux_is_empty(stack) != 0);

    /* Test automatic growth beyond eight events. */
    for (size_t i = 0; i < 20; i++)
    {
        aux_event_t item = {
            i + 100,
            STATE_WASH
        };

        CHECK(aux_push(stack, item) == 0);
    }

    CHECK(aux_count(stack) == 20);

    /* Verify last-in, first-out order. */
    for (size_t i = 20; i > 0; i--)
    {
        CHECK(aux_pop(stack, &event) == 0);
        CHECK(event.tick == (i - 1) + 100);
        CHECK(event.state == STATE_WASH);
    }

    CHECK(aux_count(stack) == 0);
    CHECK(aux_is_empty(stack) != 0);

    /* Destroy an empty stack. */
    aux_destroy(stack);

    /* Destroy a nonempty stack. */
    stack = aux_create();
    CHECK(stack != NULL);

    if (stack != NULL)
    {
        aux_event_t item = {500, STATE_DRAIN};
        CHECK(aux_push(stack, item) == 0);
        aux_destroy(stack);
    }

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_run - tests_failed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
