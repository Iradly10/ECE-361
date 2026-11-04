
#include "panel.h"
#include "iom361_r4.h"

// Detect switches that changed from OFF to ON.
uint32_t panel_pressed(uint32_t now, uint32_t before)
{
    return now & ~before;
}

// Detect switches that changed from ON to OFF.
uint32_t panel_released(uint32_t now, uint32_t before)
{
    return before & ~now;
}

// Turn one LED ON or OFF without changing the others.
int panel_led(uint32_t *base, int n, int on)
{
    int rc = 0;

    // Check if the LED number is valid.
    if (n < 0 || n > 31) {
        return -1;
    }

    // Read the current LED register.
    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    // Create a mask for the selected LED.
    uint32_t mask = (uint32_t)1u << n;

    if (on) {
        // Turn the selected LED ON.
        leds |= mask;
    } else {
        // Turn the selected LED OFF.
        leds &= ~mask;
    }

    // Write the updated value back to the LED register.
    iom361_writeReg(base, LEDS_REG, leds, &rc);

    return rc;
}

// Toggle one LED without changing the others.
int panel_toggle(uint32_t *base, int n)
{
    int rc = 0;

    // Check if the LED number is valid.
    if (n < 0 || n > 31) {
        return -1;
    }

    // Read the current LED register.
    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    // Flip only the selected LED bit.
    uint32_t mask = (uint32_t)1u << n;
    leds ^= mask;

    // Write the new LED value.
    iom361_writeReg(base, LEDS_REG, leds, &rc);

    return rc;
}
