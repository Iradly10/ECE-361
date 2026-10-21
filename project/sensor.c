#include "sensor.h"
#include "iom361_r4.h"
#include <math.h>

/* Sensor reading functions will be implemented here. */
int sensor_read(uint32_t *base, float *temp_c, float *rh_pct)
{
    int rc = 0;
uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);

if (rc != 0)
{
    return rc;
}
uint32_t raw_humid = iom361_readReg(base, HUMID_REG, &rc);
if (rc != 0)
{
    return rc;
}
// We will add the conversion formulas here.
float temperature = (raw_temp / 1048576.0f) * 200.0f - 50.0f;
float humidity = (raw_humid / 1048576.0f) * 100.0f;
temperature = roundf(temperature * 10.0f) / 10.0f;
humidity = roundf(humidity * 10.0f) / 10.0f;
}