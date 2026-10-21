#ifndef SENSOR_H
#define SENSOR_H
#include <stdint.h>
/* Sensor readings for the washing machine */
typedef struct {
    float temperature;
    float humidity;
} sensor_data_t;

int sensor_read(uint32_t *base, float *temp_c, float *rh_pct);

#endif
