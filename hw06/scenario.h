
#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>

#define SCENARIO_MAX_LINES 1000

// One line of scenario data.
typedef struct {
    unsigned int tick;
    float temp_c;
    float rh_pct;
    uint32_t switches;
} scenario_line_t;

// Stores the scenario start time and all data lines.
typedef struct {
    time_t start;
    scenario_line_t lines[SCENARIO_MAX_LINES];
    size_t count;
} scenario_t;

// Loads a scenario from a text file.
// Returns 0 on success or the first invalid file line number.
int scenario_load(const char *filename, scenario_t *scenario);

// Finds the scenario line active at a given tick.
// Values remain unchanged until the next scenario line.
const scenario_line_t *scenario_at(const scenario_t *scenario,
                                   unsigned int tick);

#endif
