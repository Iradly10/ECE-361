
#include "sensor.h"
#include "iom361_r4.h"

#include <math.h>

int sensor_read(uint32_t *base, time_t now, reading_t *r)
{
    int rc = 0;

    // Check that the output pointer is valid.
    if (r == NULL) {
        return 1;
    }

    // Read the temperature register.
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    // Read the humidity register.
    uint32_t raw_humid = iom361_readReg(base, HUMID_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    // Keep only the lower 20 bits.
    raw_temp &= 0xFFFFF;
    raw_humid &= 0xFFFFF;

    // Convert raw register values to physical measurements.
    double temp = ((double)raw_temp / 1048576.0) * 200.0 - 50.0;
    double humid = ((double)raw_humid / 1048576.0) * 100.0;

    // Round both measurements to one decimal place.
    temp = round(temp * 10.0) / 10.0;
    humid = round(humid * 10.0) / 10.0;

    // Update the reading only after both reads succeed.
    reading_t result;
    result.t = now;
    result.temp_c = (float)temp;
    result.rh_pct = (float)humid;

    *r = result;

    return 0;
}
