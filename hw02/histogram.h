#ifndef HISTOGRAM_H
#define HISTOGRAM_H

/* Prints a histogram of temperature readings.
   a contains n temperature values.
   Assumes n >= 0.
   Values outside 0 to 100 C go to the end bins. */
void histogram_print(const float a[], int n);

#endif
