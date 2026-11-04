
#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

// Stores one sensor reading with its timestamp.
typedef struct {
    time_t t;
    float temp_c;
    float rh_pct;
} reading_t;

// Reads temperature and humidity from iom361 registers.
// Returns 0 on success or an iom361 error code.
// On failure, the reading is left unchanged.
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif
