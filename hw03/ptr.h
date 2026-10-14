
#ifndef PTR_H
#define PTR_H

// Swaps the values of two floats.
// Assumes both pointers are valid.
void swap_float(float *a, float *b);

// Finds the minimum and maximum values in an array.
// Returns 0 on success, -1 for invalid input.
// On failure, min and max remain unchanged.
int minmax(const float *a, int n, float *min, float *max);

// Finds the first value greater than threshold.
// Returns its address, or NULL if none exists.
const float *find_above(const float *a, int n, float threshold);

// Reverses an array in place.
// Does nothing if a is NULL or n is less than 2.
void reverse(float *a, int n);

#endif

