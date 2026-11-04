
#include <stdio.h>
#include <stdint.h>

#include "panel.h"
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
            printf("FAIL: %s\n", #condition);                 \
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
    CHECK(panel_released(0x3u, 0x0u) == 0x0u);
    CHECK(panel_released(0x0u, 0xFu) == 0xFu);
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

    // Turn off automatic LED display output.
    iom361_set_display(false);

    // Turn LED 0 ON.
    CHECK(panel_led(base, 0, 1) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x1u);

    // Turn LED 1 ON without changing LED 0.
    CHECK(panel_led(base, 1, 1) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x3u);

    // Turn LED 0 OFF.
    CHECK(panel_led(base, 0, 0) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x2u);

    // Toggle LED 1 OFF.
    CHECK(panel_toggle(base, 1) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x0u);

    // Toggle LED 2 ON.
    CHECK(panel_toggle(base, 2) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x4u);

    // Toggle LED 2 OFF.
    CHECK(panel_toggle(base, 2) == 0);
    CHECK(iom361_readReg(base, LEDS_REG, &rc) == 0x0u);

    // Check that the register reads succeeded.
    CHECK(rc == 0);
}

// Run all tests.
int main(void)
{
    test_panel_pressed();
    test_panel_released();
    test_panel_led_invalid();
    test_panel_toggle_invalid();
    test_led_behavior();

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n",
           tests_run - tests_passed);

    return tests_run == tests_passed ? 0 : 1;
}
