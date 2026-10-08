#ifndef STATS_H
#define STATS_H

/* Returns the minimum value in an array.
   Assumes n >= 1. */
float stats_min(const float a[], int n);

/* Returns the maximum value using recursion.
   Assumes n >= 1. */
float stats_max(const float a[], int n);

/* Returns the arithmetic mean of an array.
   Assumes n >= 1. */
float stats_mean(const float a[], int n);

#endif
