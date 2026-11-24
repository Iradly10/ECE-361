# ECE 361 - Homework 9

This hash table uses separate chaining to count occurrences of nonnegative integer keys. Each bucket contains a linked list, and repeated keys increase the count of an existing node instead of creating another node.

## Build and test

- `make` builds the test executable.
- `make test` runs the tests and returns a nonzero status if any fail.
- `make memcheck` runs Valgrind if available, otherwise AddressSanitizer and UndefinedBehaviorSanitizer (with the macOS leaks tool on macOS).
- `make clean` removes generated executables.

## Table 2 explanation

With 1000 buckets, multiples of 10 use only 100 different bucket indices because their remainders are always multiples of 10, so 1000 distinct keys produce chains of length 10. With 1009 buckets, 10 and 1009 are relatively prime, so the first 1000 multiples of 10 have distinct remainders and the longest chain is 1.
