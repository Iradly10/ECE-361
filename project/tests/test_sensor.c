
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#include "../sensor.h"
#include "../iom361_r4.h"

static int tests_run = 0;
static int tests_failed = 0;

/* Fake hardware values used for testing. */
static uint32_t fake_temp = 0;
static uint32_t fake_humid = 0;
static int fake_error = 0;

/* Replace the hardware read function during tests. */
uint32_t iom361_readReg(uint32_t *base, uint32_t reg, int *rc)
{
    (void)base;

    if (rc != NULL)
    {
        *rc = fake_error;
    }

    if (fake_error != 0)
    {
        return 0;
    }

    if (reg == TEMP_REG)
    {
        return fake_temp;
    }

    if (reg == HUMID_REG)
    {
        return fake_humid;
    }

    return 0;
}

#define CHECK(condition) do {                         \
    tests_run++;                                      \
    if (condition) {                                  \
        printf("PASS: %s\n", #condition);             \
    } else {                                          \
        printf("FAIL: %s (line %d)\n",                \
               #condition, __LINE__);                 \
        tests_failed++;                               \
    }                                                 \
} while (0)

static int close_float(float a, float b)
{
    return fabsf(a - b) < 0.051f;
}

int main(void)
{
    printf("Testing Washing Machine Sensor\n\n");

    uint32_t fake_base = 0;
    reading_t reading = {0};

    /* Test 1: Reject invalid pointers. */
    CHECK(sensor_read(NULL, 100, &reading) != 0);
    CHECK(sensor_read(&fake_base, 100, NULL) != 0);

    /* Test 2: Convert raw value to 0 degrees C. */
    fake_temp = 262144;
    fake_humid = 524288;

    CHECK(sensor_read(&fake_base, 100, &reading) == 0);
    CHECK(close_float(reading.temp_c, 0.0f));
    CHECK(close_float(reading.rh_pct, 50.0f));
    CHECK(reading.timestamp == 100);

    /* Test 3: Convert raw value to 100 degrees C. */
    fake_temp = 786432;
    fake_humid = 1048576;

    CHECK(sensor_read(&fake_base, 200, &reading) == 0);
    CHECK(close_float(reading.temp_c, 100.0f));
    CHECK(close_float(reading.rh_pct, 100.0f));
    CHECK(reading.timestamp == 200);

    /* Test 4: Lowest temperature and humidity. */
    fake_temp = 0;
    fake_humid = 0;

    CHECK(sensor_read(&fake_base, 300, &reading) == 0);
    CHECK(close_float(reading.temp_c, -50.0f));
    CHECK(close_float(reading.rh_pct, 0.0f));

    /* Test 5: Heater threshold around 40 C. */
    fake_temp = 471859;
    fake_humid = 419430;

    CHECK(sensor_read(&fake_base, 400, &reading) == 0);
    CHECK(close_float(reading.temp_c, 40.0f));
    CHECK(close_float(reading.rh_pct, 40.0f));

    /* Test 6: Check one-decimal-place rounding. */
    fake_temp = 460587;
    fake_humid = 527434;

    CHECK(sensor_read(&fake_base, 500, &reading) == 0);
    CHECK(close_float(reading.temp_c, 37.9f));
    CHECK(close_float(reading.rh_pct, 50.3f));

    /* Test 7: Simulate a hardware read failure. */
    fake_error = -1;

    CHECK(sensor_read(&fake_base, 600, &reading) != 0);

    fake_error = 0;

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_run - tests_failed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
