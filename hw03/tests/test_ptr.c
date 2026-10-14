
#include <stdio.h>
#include "../ptr.h"

static int tests_run = 0;
static int tests_failed = 0;

void check_test(const char *name, int condition)
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
    float a = 5.0f;
    float b = 9.0f;

    swap_float(&a, &b);

    check_test("swap_float swaps two values",
               a == 9.0f && b == 5.0f);

    // Test minmax with multiple values
    float numbers[] = {4.0f, 8.0f, 2.0f, 9.0f, 5.0f};
    float minimum = 0.0f;
    float maximum = 0.0f;

    int result = minmax(numbers, 5, &minimum, &maximum);

    check_test("minmax multiple values",
               result == 0 && minimum == 2.0f && maximum == 9.0f);

    // Test minmax with one value
    float single[] = {7.0f};

    result = minmax(single, 1, &minimum, &maximum);

    check_test("minmax one value",
               result == 0 && minimum == 7.0f && maximum == 7.0f);

    // Test minmax with n = 0
    minimum = 100.0f;
    maximum = 200.0f;

    result = minmax(numbers, 0, &minimum, &maximum);

    check_test("minmax n equals zero",
               result == -1 && minimum == 100.0f && maximum == 200.0f);

    // Test minmax with NULL array
    result = minmax(NULL, 5, &minimum, &maximum);

    check_test("minmax NULL pointer",
               result == -1 && minimum == 100.0f && maximum == 200.0f);

    printf("\nTests run: %d\n", tests_run);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed > 0 ? 1 : 0;
}

