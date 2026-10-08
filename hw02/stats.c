#include "stats.h"

/* Find the minimum value */
float stats_min(const float a[], int n)
{
    float min = a[0];

    for (int i = 1; i < n; i++)
    {
        if (a[i] < min)
        {
            min = a[i];
        }
    }

    return min;
}

/* Find the maximum value recursively */
float stats_max(const float a[], int n)
{
    if (n == 1)
    {
        return a[0];
    }

    float rest = stats_max(a + 1, n - 1);

    return a[0] > rest ? a[0] : rest;
}

/* Calculate the mean */
float stats_mean(const float a[], int n)
{
    float sum = 0.0f;

    for (int i = 0; i < n; i++)
    {
        sum += a[i];
    }

    return sum / n;
}
