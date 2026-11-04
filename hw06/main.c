
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <stdbool.h>

#include "iom361_r4.h"
#include "sensor.h"
#include "panel.h"
#include "scenario.h"

int main(int argc, char *argv[])
{
    // The program requires one scenario filename.
    if (argc != 2) {
        fprintf(stderr, "Usage: %s scenario.txt\n", argv[0]);
        return 1;
    }

    // Load the scenario from the file.
    scenario_t scenario;

    int line_error = scenario_load(argv[1], &scenario);

    if (line_error != 0) {
        fprintf(stderr,
                "Error loading scenario at line %d\n",
                line_error);
        return 1;
    }

    // Initialize the I/O module with 4 switches and 4 LEDs.
    int rc = 0;
    uint32_t *base = iom361_initialize(4, 4, &rc);

    if (base == NULL || rc != 0) {
        fprintf(stderr, "Failed to initialize iom361\n");
        return 1;
    }

    // Disable the LED display so it does not affect the trace.
    iom361_set_display(false);

    uint32_t previous_switches = 0;

    // Find the last tick in the scenario.
    unsigned int last_tick =
        scenario.lines[scenario.count - 1].tick;

    // Process every tick from zero through the last tick.
    for (unsigned int tick = 0; ; tick++) {

        // Get the scenario values active at this tick.
        const scenario_line_t *line =
            scenario_at(&scenario, tick);

        if (line == NULL) {
            fprintf(stderr, "No scenario data for tick %u\n", tick);
            return 1;
        }

        // Update the simulated sensor and switches.
        _iom361_set_sensor1(line->temp_c, line->rh_pct);
        _iom361_set_switches(line->switches);

        // Calculate the timestamp for this tick.
        time_t now = scenario.start + (time_t)tick;

        // Read the sensor through the iom361 registers.
        reading_t reading;

        rc = sensor_read(base, now, &reading);

        if (rc != 0) {
            fprintf(stderr, "Sensor read failed: %d\n", rc);
            return 1;
        }

        // Read the current switch register.
        uint32_t switches =
            iom361_readReg(base, SWITCHES_REG, &rc);

        if (rc != 0) {
            fprintf(stderr, "Switch read failed: %d\n", rc);
            return 1;
        }

        // Find switches that were just pressed.
        uint32_t pressed =
            panel_pressed(switches, previous_switches);

        // Toggle the LED corresponding to each new press.
        for (int k = 0; k < 4; k++) {
            if ((pressed & (1u << k)) != 0) {
                rc = panel_toggle(base, k);

                if (rc != 0) {
                    fprintf(stderr,
                            "LED toggle failed: %d\n", rc);
                    return 1;
                }
            }
        }

        // Read the LED register after toggling.
        uint32_t leds =
            iom361_readReg(base, LEDS_REG, &rc);

        if (rc != 0) {
            fprintf(stderr, "LED read failed: %d\n", rc);
            return 1;
        }

        // Format the timestamp in UTC.
        struct tm *utc = gmtime(&reading.t);
        char timestamp[32];

        if (utc == NULL ||
            strftime(timestamp, sizeof(timestamp),
                     "%Y-%m-%d %H:%M:%S", utc) == 0) {
            fprintf(stderr, "Time formatting failed\n");
            return 1;
        }

        // Print one trace line for this tick.
        printf("%s temp=%.1f rh=%.1f "
               "switches=0x%x pressed=0x%x leds=0x%x\n",
               timestamp,
               reading.temp_c,
               reading.rh_pct,
               (unsigned int)switches,
               (unsigned int)pressed,
               (unsigned int)leds);

        // Save the switch state for the next tick.
        previous_switches = switches;

        if (tick == last_tick) {
            break;
        }
    }

    return 0;
}
