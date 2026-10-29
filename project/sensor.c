
#include "sensor.h"
#include "iom361_r4.h"
#include <math.h>
#include <stddef.h>

/* Read temperature and humidity from the sensor. */
int sensor_read(uint32_t *base,
                time_t timestamp,
                reading_t *reading)
{
    int rc = 0;

    if (base == NULL || reading == NULL)
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

    /* Convert the raw sensor values. */
    float temperature =
        (raw_temp / 1048576.0f) * 200.0f - 50.0f;

    float humidity =
        (raw_humid / 1048576.0f) * 100.0f;

    /* Round readings to one decimal place. */
    temperature = roundf(temperature * 10.0f) / 10.0f;
    humidity = roundf(humidity * 10.0f) / 10.0f;

    /* Save the timestamp and converted readings. */
    reading->timestamp = timestamp;
    reading->temp_c = temperature;
    reading->rh_pct = humidity;

    return 0;
}
