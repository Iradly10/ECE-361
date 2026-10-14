# ECE 361 — Homework 3

## Overview

This homework focuses on using pointers in C, including passing addresses to functions and using pointer arithmetic to work with arrays.

## Functions

The pointer library contains four functions:

- **swap_float:** Swaps the values of two float variables using pointers.
- **minmax:** Finds the minimum and maximum values in an array.
- **find_above:** Returns a pointer to the first array element greater than a specified threshold.
- **reverse:** Reverses the elements of an array using two pointers.

## Building the Program

Open a terminal in the `hw03` directory and run:

`make`

This compiles the pointer library and test program.

## Running Tests

To run the tests, enter:

`make test`

The test program displays PASS or FAIL for each test, followed by the total number of tests and failures.

## Cleaning Compiled Files

To remove generated object files and the test executable, enter:

`make clean`

## Test Results

The initial 10 tests passed with 0 failures.

## Files

- `ptr.h` — Function declarations
- `ptr.c` — Pointer function implementations
- `tests/test_ptr.c` — Unit tests
- `Makefile` — Build and test instructions
- `README.md` — Homework documentation
- `AI_USAGE.md` — AI usage documentation
