# ECE 361 - Homework 4

## Description

This program uses a tally system to count how many times different names appear in an input. It keeps the names in alphabetical order and tracks how many times each name appears.

The program can store up to 16 different names in each tally, and each name can have up to 15 characters. It also supports up to 4 active tallies at the same time.

## How to Build

To compile the program, open the terminal inside the `hw04` folder and type:

```bash
make
```

## How to Run

To run the program, type:

```bash
./events
```

You can enter names one at a time. Press `Control + D` when you're finished entering names.

## How to Test

To run the automated tests, type:

```bash
make test
```

The tests check that the tally functions work correctly, including counting names, rejecting invalid inputs, and handling the tally limits.

## Opaque Type Explanation

The `tally_t` type is called an opaque type because `tally.h` only tells us that the structure exists, but it does not show what's inside it.

The actual structure is defined in `tally.c`, so other files cannot directly access its members. Instead, they must use functions such as `tally_add()`, `tally_count()`, and `tally_size()`.

This helps protect the data and prevents other parts of the program from accidentally changing the tally's internal information.