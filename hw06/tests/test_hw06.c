
#include <stdio.h>
#include <stdint.h>

#include "../panel.h"

// Keep track of the test results.
static int tests_run = 0;
static int tests_passed = 0;

// Check whether each test passes or fails.
#define CHECK(condition) do {                         \
    tests_run++;                                       \
    if (condition) {                                   \
        tests_passed++;                                \
        printf("PASS: %s\n", #condition);              \
    } else {                                           \
        printf("FAIL: %s (line %d)\n",                  \
               #condition, __LINE__);                  \
    }                                                  \
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

// Test invalid LED numbers for panel_led.
static void test_panel_led_errors(void)
{
    printf("\nTesting panel_led invalid inputs...\n");

    // Valid LED numbers are 0 through 31.
    CHECK(panel_led(NULL, -1, 1) == -1);
    CHECK(panel_led(NULL, 32, 1) == -1);
    CHECK(panel_led(NULL, 100, 0) == -1);
}

// Test invalid LED numbers for panel_toggle.
static void test_panel_toggle_errors(void)
{
    printf("\nTesting panel_toggle invalid inputs...\n");

    // Valid LED numbers are 0 through 31.
    CHECK(panel_toggle(NULL, -1) == -1);
    CHECK(panel_toggle(NULL, 32) == -1);
    CHECK(panel_toggle(NULL, 100) == -1);
}

// Run all unit tests.
int main(void)
{
    test_panel_pressed();
    test_panel_released();
    test_panel_led_errors();
    test_panel_toggle_errors();

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n",
           tests_run - tests_passed);

    return tests_run == tests_passed ? 0 : 1;
}
