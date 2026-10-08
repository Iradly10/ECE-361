#include "fsm.h"

/* Return the name of the current washing machine state. */
const char *fsm_state_name(washer_state_t state)
{
    switch (state)
    {
        case STATE_IDLE:
            return "IDLE";

        case STATE_FILL:
            return "FILL";

        case STATE_HEAT:
            return "HEAT";

        case STATE_WASH:
            return "WASH";

        case STATE_DRAIN:
            return "DRAIN";

        case STATE_DONE:
            return "DONE";

        default:
            return "UNKNOWN";
    }
}
