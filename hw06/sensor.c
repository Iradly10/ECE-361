
#include "sensor.h"
#include "iom361_r4.h"

#include <math.h>
#include <stddef.h>

int sensor_read(uint32_t *base, time_t now, reading_t *r)
{
    int rc = 0;

    // Make sure the reading pointer is valid.
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

    // Keep only bits 19 through 0.
    raw_temp &= 0xFFFFFu;
    raw_humid &= 0xFFFFFu;

    // Convert the raw values.
    double temp = ((double)raw_temp / 1048576.0) * 200.0 - 50.0;
    double humid = ((double)raw_humid / 1048576.0) * 100.0;

    // Round to one decimal place.
    temp = round(temp * 10.0) / 10.0;
    humid = round(humid * 10.0) / 10.0;

    // Save the results only after both reads succeed.
    reading_t result;

    result.t = now;
    result.temp_c = (float)temp;
    result.rh_pct = (float)humid;

    *r = result;

    return 0;
}
