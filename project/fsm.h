
#ifndef FSM_H
#define FSM_H

/* Washing machine states */
typedef enum {
    STATE_IDLE,
    STATE_FILL,
    STATE_WASH,
    STATE_RINSE,
    STATE_SPIN,
    STATE_DRAIN,
    STATE_COUNT
} washer_state_t;

/* Washing machine events */
typedef enum {
    EVENT_START,
    EVENT_FILL_COMPLETE,
    EVENT_WASH_COMPLETE,
    EVENT_RINSE_COMPLETE,
    EVENT_SPIN_COMPLETE,
    EVENT_DRAIN_COMPLETE,
    EVENT_DOOR_OPEN,
    EVENT_DOOR_CLOSED,
    EVENT_COUNT
} washer_event_t;

/* Actions performed during transitions */
typedef enum {
    ACTION_NONE,
    ACTION_START_FILL,
    ACTION_START_WASH,
    ACTION_START_RINSE,
    ACTION_START_SPIN,
    ACTION_START_DRAIN,
    ACTION_FINISH,
    ACTION_STOP
} washer_action_t;

/* Transition table entry */
typedef struct {
    washer_state_t next_state;
    washer_action_t action;
} fsm_transition_t;

/* FSM functions */
const char *fsm_state_name(washer_state_t state);

washer_state_t fsm_next_state(
    washer_state_t state,
    washer_event_t event
);

fsm_transition_t fsm_get_transition(
    washer_state_t state,
    washer_event_t event
);

#endif
