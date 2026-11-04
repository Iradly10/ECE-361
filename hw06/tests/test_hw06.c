
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>

#include "panel.h"
#include "sensor.h"
#include "scenario.h"
#include "iom361_r4.h"

// Keep track of test results.
static int tests_run = 0;
static int tests_passed = 0;

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

// Test LED ON, OFF, and toggle behavior.
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

    // Normal sensor values.
    _iom361_set_sensor1(21.5f, 40.0f);

    CHECK(sensor_read(base, timestamp, &reading) == 0);
    CHECK(reading.temp_c == 21.5f);
    CHECK(reading.rh_pct == 40.0f);
    CHECK(reading.t == timestamp);

    // Test 91 degrees Celsius.
    _iom361_set_sensor1(91.0f, 55.0f);

    CHECK(sensor_read(base, timestamp, &reading) == 0);
    CHECK(reading.temp_c == 91.0f);
    CHECK(reading.rh_pct == 55.0f);

    // Test rounding.
    _iom361_set_sensor1(23.46f, 67.84f);

    CHECK(sensor_read(base, timestamp, &reading) == 0);
    CHECK(reading.temp_c == 23.5f);
    CHECK(reading.rh_pct == 67.8f);

    // Failed read must preserve previous values.
    reading_t saved = reading;

    CHECK(sensor_read(NULL, timestamp, &reading) != 0);
    CHECK(reading.t == saved.t &&
          reading.temp_c == saved.temp_c &&
          reading.rh_pct == saved.rh_pct);

    CHECK(sensor_read(base, timestamp, NULL) != 0);
}

// Test loading a valid scenario file.
static void test_scenario_load(void)
{
    printf("\nTesting scenario_load...\n");

    scenario_t scenario = {0};

    CHECK(scenario_load("scenarios/basic.txt", &scenario) == 0);

    CHECK(scenario.count == 4);

    CHECK(scenario.lines[0].tick == 0);
    CHECK(scenario.lines[0].temp_c == 21.5f);
    CHECK(scenario.lines[0].rh_pct == 40.0f);
    CHECK(scenario.lines[0].switches == 0x0u);

    CHECK(scenario.lines[1].tick == 2);
    CHECK(scenario.lines[1].switches == 0x1u);

    CHECK(scenario.lines[2].tick == 4);
    CHECK(scenario.lines[2].temp_c == 22.0f);

    CHECK(scenario.lines[3].tick == 6);
    CHECK(scenario.lines[3].switches == 0x2u);
}

// Test finding scenario data by tick.
static void test_scenario_at(void)
{
    printf("\nTesting scenario_at...\n");

    scenario_t scenario = {0};

    if (scenario_load("scenarios/basic.txt", &scenario) != 0) {
        CHECK(0);
        return;
    }

    const scenario_line_t *line;

    line = scenario_at(&scenario, 0);
    CHECK(line != NULL && line->tick == 0);

    line = scenario_at(&scenario, 1);
    CHECK(line != NULL && line->tick == 0);

    line = scenario_at(&scenario, 2);
    CHECK(line != NULL && line->tick == 2);

    line = scenario_at(&scenario, 3);
    CHECK(line != NULL && line->tick == 2);

    line = scenario_at(&scenario, 4);
    CHECK(line != NULL && line->tick == 4);

    line = scenario_at(&scenario, 6);
    CHECK(line != NULL && line->tick == 6);

    line = scenario_at(&scenario, 100);
    CHECK(line != NULL && line->tick == 6);

    CHECK(scenario_at(NULL, 0) == NULL);
}

// Test invalid scenarios and error handling.
static void test_scenario_invalid(void)
{
    printf("\nTesting invalid scenarios...\n");

    scenario_t scenario = {0};

    // Load a valid scenario first.
    CHECK(scenario_load("scenarios/basic.txt",
                        &scenario) == 0);

    // Save original data.
    size_t saved_count = scenario.count;
    time_t saved_start = scenario.start;
    unsigned int saved_tick = scenario.lines[0].tick;

    // Duplicate tick on line 5 must be rejected.
    CHECK(scenario_load(
        "scenarios/invalid_duplicate_tick.txt",
        &scenario) == 5);

    // A failed load must preserve existing data.
    CHECK(scenario.count == saved_count);
    CHECK(scenario.start == saved_start);
    CHECK(scenario.lines[0].tick == saved_tick);

    // Missing file must fail.
    CHECK(scenario_load(
        "scenarios/does_not_exist.txt",
        &scenario) != 0);

    // NULL filename must fail.
    CHECK(scenario_load(NULL, &scenario) != 0);

    // NULL scenario pointer must fail.
    CHECK(scenario_load(
        "scenarios/basic.txt", NULL) != 0);

    // Empty scenario has no entries.
    scenario_t empty = {0};

    CHECK(scenario_at(&empty, 0) == NULL);
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
    test_scenario_load();
    test_scenario_at();
    test_scenario_invalid();

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n",
           tests_run - tests_passed);

    return tests_run == tests_passed ? 0 : 1;
}
