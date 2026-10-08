#ifndef READING_H
#define READING_H

#include <stdio.h>

#define MAX_READINGS 1000

/* Reads sensor data from input.
   Stores ticks, temperatures, and humidity.
   Assumes arrays have room for MAX_READINGS values.
   Returns the number of valid readings.
   Writes the number of invalid lines to skipped. */
int reading_read(FILE *input,
                 int ticks[],
                 float temps[],
                 float hums[],
                 int *skipped);

#endif

