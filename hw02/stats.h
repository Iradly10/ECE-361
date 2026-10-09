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

/* Finds the longest consecutive run strictly above threshold.
   a contains n values; n >= 0.
   Writes the starting index through start_index.
   Returns the length of the longest run.
   If no run exists, sets *start_index to -1. */
int stats_longest_run_above(const float a[], int n,
                            float threshold, int *start_index);


float stats_max_tail(const float a[], int n);
                         
#endif
