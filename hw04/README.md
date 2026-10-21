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

The program ignores empty lines and lines starting with `#`. It prints the names in alphabetical order with their counts and shows how many names were counted and how many input lines were skipped.

## How to Test

To run the automated tests, type:

```bash
make test
```

The tests check that the tally functions work correctly, including counting names, rejecting invalid inputs, handling the tally limits, reusing destroyed tallies, and printing names in alphabetical order.

When I ran the tests, I got:

```text
Tests run: 26, failed: 0
```

## Opaque Type Explanation

The `tally_t` type is called an opaque type because `tally.h` only tells us that the structure exists, but it does not show what's inside it.

The actual structure is defined in `tally.c`, so other files cannot directly access its members. Instead, they must use functions such as `tally_add()`, `tally_count()`, and `tally_size()`.

This helps protect the data and prevents other parts of the program from accidentally changing the tally's internal information.

## Compiler Experiment

I created a temporary file called `opaque_test.c` that included `tally.h`. I tried to access the tally structure directly using:

```c
int count = t->size;
```

I compiled the file using:

```bash
gcc -std=c11 -Wall -Wextra -c opaque_test.c
```

The compiler gave me this error:

```text
opaque_test.c:6:18: error: incomplete definition of type 'struct tally'
```

It also showed a note about the forward declaration of `struct tally` in `tally.h`.

This happened because `tally.h` only declares the structure but does not define its members. The structure is defined inside `tally.c`, so other files cannot directly access `t->size`.

This experiment helped me understand why opaque types are useful. They hide the internal information and make other files use the functions provided by the program instead.