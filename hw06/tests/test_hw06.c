
#include <stdio.h>
#include <stdint.h>

#include "../panel.h"

static int tests_run = 0;
static int tests_passed = 0;

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

int main(void)
{
    test_panel_pressed();
    test_panel_released();

    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n",
           tests_run - tests_passed);

    return tests_run == tests_passed ? 0 : 1;
}
