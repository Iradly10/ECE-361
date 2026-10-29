
#include "status.h"
#include "bits.h"

/*
 * Pack the washing machine state, LED values,
 * and door-lock status into a 32-bit integer.
 *
 * Bits 0-2: Washing machine state
 * Bits 3-6: LED values
 * Bit 7: Door locked
 * Bits 8-31: Unused (set to zero)
 */
uint32_t pack_status(washer_state_t state,
                     uint32_t leds)
{
    uint32_t status = 0;

    status = set_field(status, 0, 3, (uint32_t)state);
    status = set_field(status, 3, 4, leds);
    status = set_field(status, 7, 1,
                       state != STATE_IDLE ? 1u : 0u);

    return status;
}

/*
 * Unpack the status word using the
 * get_field() function from Homework 1.
 */
void unpack_status(uint32_t status,
                   uint32_t *state,
                   uint32_t *leds,
                   uint32_t *locked)
{
    *state = get_field(status, 0, 3);
    *leds = get_field(status, 3, 4);
    *locked = get_field(status, 7, 1);
}
