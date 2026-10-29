
#include "sensor.h"
#include "iom361_r4.h"
#include <math.h>
#include <stddef.h>

/* Read temperature and humidity from the sensor. */
int sensor_read(uint32_t *base, float *temp_c, float *rh_pct)
{
    int rc = 0;

    if (base == NULL || temp_c == NULL || rh_pct == NULL)
    {
        return -1;
    }

    /* Read the raw temperature value. */
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);

    if (rc != 0)
    {
        return rc;
    }

    /* Read the raw humidity value. */
    uint32_t raw_humid = iom361_readReg(base, HUMID_REG, &rc);

    if (rc != 0)
    {
        return rc;
    }

    /* Convert the raw readings into real values. */
    float temperature = (raw_temp / 1048576.0f) * 200.0f - 50.0f;
    float humidity = (raw_humid / 1048576.0f) * 100.0f;

    /* Round both values to one decimal place. */
    temperature = roundf(temperature * 10.0f) / 10.0f;
    humidity = roundf(humidity * 10.0f) / 10.0f;

    /* Store the converted values. */
    *temp_c = temperature;
    *rh_pct = humidity;

    /* Return zero when the sensor readings succeed. */
    return 0;
}
