
#include "fsm.h"

/* Transition table indexed by state and event. */
static const fsm_transition_t transitions[STATE_COUNT][EVENT_COUNT] = {
    [STATE_IDLE] = {
        [EVENT_START] = {STATE_FILL, ACTION_START_FILL}
    },
    [STATE_FILL] = {
        [EVENT_FILL_COMPLETE] = {STATE_WASH, ACTION_START_WASH},
        [EVENT_DOOR_OPEN] = {STATE_IDLE, ACTION_STOP}
    },
    [STATE_WASH] = {
        [EVENT_WASH_COMPLETE] = {STATE_RINSE, ACTION_START_RINSE},
        [EVENT_DOOR_OPEN] = {STATE_IDLE, ACTION_STOP}
    },
    [STATE_RINSE] = {
        [EVENT_RINSE_COMPLETE] = {STATE_SPIN, ACTION_START_SPIN},
        [EVENT_DOOR_OPEN] = {STATE_IDLE, ACTION_STOP}
    },
    [STATE_SPIN] = {
        [EVENT_SPIN_COMPLETE] = {STATE_DRAIN, ACTION_START_DRAIN},
        [EVENT_DOOR_OPEN] = {STATE_IDLE, ACTION_STOP}
    },
    [STATE_DRAIN] = {
        [EVENT_DRAIN_COMPLETE] = {STATE_IDLE, ACTION_FINISH},
        [EVENT_DOOR_OPEN] = {STATE_IDLE, ACTION_STOP}
    }
};

/* Return the name of the current state. */
const char *fsm_state_name(washer_state_t state)
{
    switch (state)
    {
        case STATE_IDLE:  return "IDLE";
        case STATE_FILL:  return "FILL";
        case STATE_WASH:  return "WASH";
        case STATE_RINSE: return "RINSE";
        case STATE_SPIN:  return "SPIN";
        case STATE_DRAIN: return "DRAIN";
        default:          return "UNKNOWN";
    }
}

/* Look up the next state and action. */
fsm_transition_t fsm_get_transition(
    washer_state_t state,
    washer_event_t event
)
{
    fsm_transition_t result = {state, ACTION_NONE};

    if (state < STATE_IDLE || state >= STATE_COUNT ||
        event < EVENT_START || event >= EVENT_COUNT)
    {
        return result;
    }

    fsm_transition_t entry = transitions[state][event];

    if (entry.action != ACTION_NONE)
    {
        return entry;
    }

    return result;
}

/* Return the next state after an event. */
washer_state_t fsm_next_state(
    washer_state_t state,
    washer_event_t event
)
{
    return fsm_get_transition(state, event).next_state;
}
