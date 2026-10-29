
#ifndef LOCK_H
#define LOCK_H

/* The four possible states of the lock */
typedef enum {
    LOCKED,
    GOT1,
    GOT2,
    OPEN,
    N_STATES
} state_t;

/* Events that can happen to the lock */
typedef enum {
    E_RIGHT,
    E_WRONG,
    E_CLOSE,
    E_TIMEOUT,
    N_EVENTS
} event_t;

/* Actions the lock can perform */
typedef enum {
    A_UNDEFINED,
    A_NONE,
    A_UNLOCK,
    A_LOCK,
    A_BEEP,
    N_ACTIONS
} action_t;

/* Each transition has a next state and an action */
typedef struct {
    state_t next;
    action_t action;
} transition_t;

/* Table containing all state transitions */
extern const transition_t lock_table[N_STATES][N_EVENTS];

/* Finds the next state and action for an event */
state_t lock_step(state_t s, event_t e, action_t *action);

/* Converts an input character into an event */
int lock_event(state_t s, char c);

/* Returns the name of a state */
const char *state_name(state_t s);

/* Returns the name of an event */
const char *event_name(event_t e);

/* Returns the name of an action */
const char *action_name(action_t a);

#endif
