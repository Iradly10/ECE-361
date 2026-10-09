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


/* Helper function for tail-recursive maximum */
static float max_tail_helper(const float a[], int n, float best)
{
    if (n == 0)
    {
        return best;
    }

    if (a[0] > best)
    {
        best = a[0];
    }

    return max_tail_helper(a + 1, n - 1, best);
}

/* Find the maximum using tail recursion */
float stats_max_tail(const float a[], int n)
{
    if (n <= 0)
    {
        return 0.0f;
    }

    return max_tail_helper(a + 1, n - 1, a[0]);
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


/* Find the longest run above a threshold */
int stats_longest_run_above(const float a[], int n,
                            float threshold, int *start_index)
{
    int run = 0;
    int best = 0;
    int start = 0;
    int best_start = -1;

    for (int i = 0; i < n; i++)
    {
        if (a[i] > threshold)
        {
            if (run == 0)
            {
                start = i;
            }

            run++;

            if (run > best)
            {
                best = run;
                best_start = start;
            }
        }
        else
        {
            run = 0;
        }
    }

    *start_index = best_start;
    return best;
}
