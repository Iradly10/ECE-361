
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

    printf("\nTests run: %d\n", tests_run);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed > 0 ? 1 : 0;
}

