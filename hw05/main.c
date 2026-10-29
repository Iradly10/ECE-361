
#include <stdio.h>
#include "lock.h"

/* Main program for the combination lock */
int main(void)
{
    state_t state = LOCKED;
    action_t action;
    int ch;

    /* Read input one character at a time */
    while ((ch = getchar()) != EOF) {

        /* Convert the character into an event */
        int event = lock_event(state, (char)ch);

        /* Skip characters that are not valid events */
        if (event == -1) {
            continue;
        }

        /* Save the current state before changing it */
        state_t old_state = state;

        /* Find the next state using the transition table */
        state = lock_step(state, (event_t)event, &action);

        /* Print the state, input, event, next state, and action */
        printf("%s %c %s -> %s %s\n",
               state_name(old_state),
               (char)ch,
               event_name((event_t)event),
               state_name(state),
               action_name(action));
    }

    /* Print the final state */
    printf("final: %s\n", state_name(state));

    return 0;
}
