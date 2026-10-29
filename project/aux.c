
#include "aux.h"
#include <stdlib.h>
#include <stdint.h>

#define INITIAL_CAPACITY 8

/* Internal structure of the event stack. */
struct aux {
    aux_event_t *events;
    size_t count;
    size_t capacity;
};

/* Create a stack with room for 8 events. */
aux_t *aux_create(void)
{
    aux_t *stack = malloc(sizeof(*stack));

    if (stack == NULL)
        return NULL;

    stack->events = malloc(
        INITIAL_CAPACITY * sizeof(*stack->events)
    );

    if (stack->events == NULL)
    {
        free(stack);
        return NULL;
    }

    stack->count = 0;
    stack->capacity = INITIAL_CAPACITY;

    return stack;
}

/* Push an event onto the stack. */
int aux_push(aux_t *stack, aux_event_t event)
{
    if (stack == NULL)
        return -1;

    if (stack->count == stack->capacity)
    {
        if (stack->capacity > SIZE_MAX / 2 ||
            stack->capacity * 2 >
                SIZE_MAX / sizeof(*stack->events))
        {
            return -1;
        }

        size_t new_capacity = stack->capacity * 2;

        aux_event_t *new_events = realloc(
            stack->events,
            new_capacity * sizeof(*stack->events)
        );

        if (new_events == NULL)
            return -1;

        stack->events = new_events;
        stack->capacity = new_capacity;
    }

    stack->events[stack->count] = event;
    stack->count++;

    return 0;
}

/* Remove the most recent event. */
int aux_pop(aux_t *stack, aux_event_t *event)
{
    if (stack == NULL || event == NULL ||
        stack->count == 0)
    {
        return -1;
    }

    stack->count--;
    *event = stack->events[stack->count];

    return 0;
}

/* Read the most recent event without removing it. */
int aux_peek(const aux_t *stack, aux_event_t *event)
{
    if (stack == NULL || event == NULL ||
        stack->count == 0)
    {
        return -1;
    }

    *event = stack->events[stack->count - 1];

    return 0;
}

/* Return the number of events stored. */
size_t aux_count(const aux_t *stack)
{
    if (stack == NULL)
        return 0;

    return stack->count;
}

/* Check whether the stack is empty. */
int aux_is_empty(const aux_t *stack)
{
    return stack == NULL || stack->count == 0;
}

/* Free the stack and its memory. */
void aux_destroy(aux_t *stack)
{
    if (stack == NULL)
        return;

    free(stack->events);
    free(stack);
}
