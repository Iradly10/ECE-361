#include "ptr.h"
#include <stddef.h>

void swap_float(float *a, float *b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

int minmax(const float *a, int n, float *min, float *max)
{
    if (a == NULL || min == NULL || max == NULL || n < 1)
    {
        return -1;
    }

    float smallest = *a;
    float largest = *a;

    const float *p = a + 1;

    while (p < a + n)
    {
        if (*p < smallest)
        {
            smallest = *p;
        }

        if (*p > largest)
        {
            largest = *p;
        }

        p++;
    }

    *min = smallest;
    *max = largest;

    return 0;
}

