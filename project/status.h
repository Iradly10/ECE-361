
#ifndef STATUS_H
#define STATUS_H

#include <stdint.h>
#include "fsm.h"

/*
 * Pack the washing machine state, LED values,
 * and door-lock status into a 32-bit integer.
 */
uint32_t pack_status(washer_state_t state, uint32_t leds);

/*
 * Unpack the status word to retrieve the
 * state, LED values, and door-lock status.
 */
void unpack_status(uint32_t status,
                   uint32_t *state,
                   uint32_t *leds,
                   uint32_t *locked);

#endif
