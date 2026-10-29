
#include "lock.h"

/* Table showing how the lock changes states */
const transition_t lock_table[N_STATES][N_EVENTS] = {
    [LOCKED] = {
        [E_RIGHT]   = {GOT1, A_NONE},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {LOCKED, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },

    [GOT1] = {
        [E_RIGHT]   = {GOT2, A_NONE},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {GOT1, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },

    [GOT2] = {
        [E_RIGHT]   = {OPEN, A_UNLOCK},
        [E_WRONG]   = {LOCKED, A_BEEP},
        [E_CLOSE]   = {GOT2, A_NONE},
        [E_TIMEOUT] = {LOCKED, A_NONE}
    },

    [OPEN] = {
        [E_RIGHT]   = {OPEN, A_NONE},
        [E_WRONG]   = {OPEN, A_NONE},
        [E_CLOSE]   = {LOCKED, A_LOCK},
        [E_TIMEOUT] = {LOCKED, A_LOCK}
    }
};

/* Uses the table to find the next state and action */
state_t lock_step(state_t s, event_t e, action_t *action)
{
    transition_t result = lock_table[s][e];

    *action = result.action;

    return result.next;
}

/* Converts a character into an event */
int lock_event(state_t s, char c)
{
    /* Check if the door is closing */
    if (c == 'c') {
        return E_CLOSE;
    }

    /* Check if the timer runs out */
    if (c == 't') {
        return E_TIMEOUT;
    }

    /* Check if the character is a digit */
    if (c >= '0' && c <= '9') {

        /* Digits do nothing while the door is open */
        if (s == OPEN) {
            return E_WRONG;
        }

        /* First correct digit is 3 */
        if (s == LOCKED && c == '3') {
            return E_RIGHT;
        }

        /* Second correct digit is 1 */
        if (s == GOT1 && c == '1') {
            return E_RIGHT;
        }

        /* Third correct digit is 4 */
        if (s == GOT2 && c == '4') {
            return E_RIGHT;
        }

        /* Any other digit is wrong */
        return E_WRONG;
    }

    /* Ignore characters that are not recognized */
    return -1;
}

/* Returns the name of the current state */
const char *state_name(state_t s)
{
    static const char *names[N_STATES] = {
        "LOCKED",
        "GOT1",
        "GOT2",
        "OPEN"
    };

    if (s < 0 || s >= N_STATES) {
        return "?";
    }

    return names[s];
}

/* Returns the name of an event */
const char *event_name(event_t e)
{
    static const char *names[N_EVENTS] = {
        "RIGHT",
        "WRONG",
        "CLOSE",
        "TIMEOUT"
    };

    if (e < 0 || e >= N_EVENTS) {
        return "?";
    }

    return names[e];
}

/* Returns the name of an action */
const char *action_name(action_t a)
{
    static const char *names[N_ACTIONS] = {
        "UNDEFINED",
        "NONE",
        "UNLOCK",
        "LOCK",
        "BEEP"
    };

    if (a < 0 || a >= N_ACTIONS) {
        return "?";
    }

    return names[a];
}
