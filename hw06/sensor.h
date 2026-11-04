
#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

// Structure for storing sensor readings.
typedef struct {
    time_t t;
    float temp_c;
    float rh_pct;
} reading_t;

// Reads temperature and humidity from iom361 registers.
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif
