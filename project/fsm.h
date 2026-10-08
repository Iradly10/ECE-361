#ifndef FSM_H
#define FSM_H

/* Washing machine operating states */
typedef enum {
    STATE_IDLE,
    STATE_FILL,
    STATE_HEAT,
    STATE_WASH,
    STATE_DRAIN,
    STATE_DONE
} washer_state_t;

#endif