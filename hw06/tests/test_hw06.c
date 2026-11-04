
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>

#include "panel.h"
#include "sensor.h"
#include "iom361_r4.h"

// Keep track of test results.
static int tests_run = 0;
static int tests_passed = 0;

// Check whether a test passes or fails.
#define CHECK(condition)                                      \
    do {                                                      \
        tests_run++;                                          \
        if (condition) {                                      \
            tests_passed++;                                   \
            printf("PASS: %s\n", #condition);                 \
        } else {                                              \
            printf("FAIL: %s (line %d)\n",                     \
                   #condition, __LINE__);                     \
        }                                                     \
    } while (0)

// Test switches changing from OFF to ON.
static void test_panel_pressed(void)
{
    printf("\nTesting panel_pressed...\n");

    CHECK(panel_pressed(0x0u, 0x0u) == 0x0u);
    CHECK(panel_pressed(0x1u, 0x0u) == 0x1u);
    CHECK(panel_pressed(0x3u, 0x1u) == 0x2u);
    CHECK(panel_pressed(0x1u, 0x1u) == 0x0u);
    CHECK(panel_pressed(0x0u, 0x1u) == 0x0u);
    CHECK(panel_pressed(0xFu, 0x0u) == 0xFu);
}

// Test switches changing from ON to OFF.
static void test_panel_released(void)
{
    printf("\nTesting panel_released...\n");

    CHECK(panel_released(0x0u, 0x0u) == 0x0u);
    CHECK(panel_released(0x0u, 0x1u) == 0x1u);
    CHECK(panel_released(0x1u, 0x3u) == 0x2u);
    CHECK(panel_released(0x1u, 0x1u) == 0x0u);
    CHECK(panel_released(0x0u, 0xFu) == 0xFu);
    CHECK(panel_released(0x3u, 0x0u) == 0x0u);
}

// Test invalid LED numbers.
static void test_panel_led_invalid(void)
{
    printf("\nTesting panel_led invalid inputs...\n");

    CHECK(panel_led(NULL, -1, 1) == -1);
    CHECK(panel_led(NULL, 32, 1) == -1);
    CHECK(panel_led(NULL, 100, 0) == -1);
}

// Test invalid LED toggle numbers.
static void test_panel_toggle_invalid(void)
{
    printf("\nTesting panel_toggle invalid inputs...\n");

    CHECK(panel_toggle(NULL, -1) == -1);
    CHECK(panel_toggle(NULL, 32) == -1);
    CHECK(panel_toggle(NULL, 100) == -1);
}

// Test turning LEDs ON, OFF, and toggling.
static void test_led_behavior(void)
{
    int rc = 0;

    uint32_t *base = iom361_initialize(8, 8, &rc);

    printf("\nTesting LED behavior...\n");

    if (base == NULL || rc != 0) {
        CHECK(base != NULL && rc == 0);
        return;
    }

    iom361_set_display(false);

    CHECK(panel_led(base, 0, 1) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x1u);

    CHECK(panel_led(base, 1, 1) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x3u);

    CHECK(panel_led(base, 0, 0) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x2u);

    CHECK(panel_toggle(base, 1) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x0u);

    CHECK(panel_toggle(base, 2) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x4u);

    CHECK(panel_toggle(base, 2) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x0u);

    CHECK(rc == 0);
}

// Test sensor temperature, humidity, and errors.
static void test_sensor_read(void)
{
    int rc = 0;

    printf("\nTesting sensor_read...\n");

    uint32_t *base = iom361_initialize(8, 8, &rc);

    if (base == NULL || rc != 0) {
        CHECK(base != NULL && rc == 0);
        return;
    }

    reading_t reading = {0};

    time_t timestamp = (time_t)1000;

    // Test normal temperature and humidity.
    _iom361_set_sensor1(21.5f, 40.0f);

    CHECK(sensor_read(base, timestamp, &reading) == 0);
    CHECK(reading.temp_c == 21.5f);
    CHECK(reading.rh_pct == 40.0f);
    CHECK(reading.t == timestamp);

    // Test temperature of 91 degrees Celsius.
    _iom361_set_sensor1(91.0f, 55.0f);

    CHECK(sensor_read(base, timestamp, &reading) == 0);
    CHECK(reading.temp_c == 91.0f);
    CHECK(reading.rh_pct == 55.0f);

    // Test rounding to one decimal place.
    _iom361_set_sensor1(23.46f, 67.84f);

    CHECK(sensor_read(base, timestamp, &reading) == 0);
    CHECK(reading.temp_c == 23.5f);
    CHECK(reading.rh_pct == 67.8f);

    // Test that an invalid base does not change the reading.
    reading_t saved = reading;

    CHECK(sensor_read(NULL, timestamp, &reading) != 0);
    CHECK(reading.t == saved.t &&
          reading.temp_c == saved.temp_c &&
          reading.rh_pct == saved.rh_pct);

    // Test a NULL output pointer.
    CHECK(sensor_read(base, timestamp, NULL) != 0);
}

// Run all tests.
int main(void)
{
    test_panel_pressed();
    test_panel_released();
    test_panel_led_invalid();
    test_panel_toggle_invalid();
    test_led_behavior();
    test_sensor_read();

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n",
           tests_run - tests_passed);

    return tests_run == tests_passed ? 0 : 1;
}
