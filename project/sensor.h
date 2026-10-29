
#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

/* Timestamped temperature and humidity reading. */
typedef struct {
    time_t timestamp;
    float temp_c;
    float rh_pct;
} reading_t;

/* Read temperature and humidity from the hardware. */
int sensor_read(uint32_t *base,
                time_t timestamp,
                reading_t *reading);

#endif
