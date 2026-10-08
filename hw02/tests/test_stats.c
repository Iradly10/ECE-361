
#include <stdio.h>
#include "../stats.h"

static int tests_run = 0;
static int tests_failed = 0;

static void check_test(const char *name, int condition)
{
    tests_run++;

    if (condition)
    {
        printf("PASS: %s\n", name);
    }
    else
    {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}

int main(void)
{
    /* Test 1: Array with one value */
    float single[] = {15.0f};

    check_test("Single value minimum",
               stats_min(single, 1) == 15.0f);

    check_test("Single value maximum",
               stats_max(single, 1) == 15.0f);

    check_test("Single value mean",
               stats_mean(single, 1) == 15.0f);

    /* Test 2: Maximum at beginning */
    float first[] = {50.0f, 20.0f, 10.0f};

    check_test("Maximum at beginning",
               stats_max(first, 3) == 50.0f);

    /* Test 3: Maximum at end */
    float last[] = {10.0f, 20.0f, 50.0f};

    check_test("Maximum at end",
               stats_max(last, 3) == 50.0f);

    /* Test 4: Value equal to threshold ends run */
    float values[] = {30.0f, 35.0f, 25.0f,
                      40.0f, 45.0f};

    int start = -1;

    int run = stats_longest_run_above(
        values, 5, 25.0f, &start);

    check_test("Threshold equality ends run",
               run == 2 && start == 3);

    /* Test 5: Recursive maximum of 10000 values */
    float large[10000];

    for (int i = 0; i < 10000; i++)
    {
        large[i] = (float)i;
    }

    check_test("Recursive maximum 10000 values",
               stats_max(large, 10000) == 9999.0f);

    /* Summary */
    printf("\nTests run: %d\n", tests_run);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
