
# ECE 361 - Homework 6

## Overview

For Homework 6, I worked on a C program that simulates temperature,
humidity, switches, and LEDs using the iom361 hardware emulator.

The program reads scenario files and displays how the sensor values,
switches, and LEDs change over time.

## Files

- `main.c` - Runs the replay simulation.
- `sensor.c` / `sensor.h` - Reads temperature and humidity.
- `panel.c` / `panel.h` - Detects switch changes and controls LEDs.
- `scenario.c` / `scenario.h` - Loads scenario files.
- `iom361_r4.c` / `iom361_r4.h` - Hardware emulator.
- `tests/test_hw06.c` - Unit tests.
- `scenarios/` - Scenario input files.
- `expected/` - Expected replay output.
- `regress.sh` - Regression testing script.
- `Makefile` - Builds and tests the program.

## How to Build

To compile the program, I open my terminal inside the hw06 folder
and run:

    make

This creates the executable file named `replay`.

## How to Run

To run the basic scenario, I use:

    ./replay scenarios/basic.txt

The program displays the timestamp, temperature, humidity,
switch values, newly pressed switches, and LED states.

## How to Test

To run all the unit and regression tests, I use:

    make test

To check everything from a clean build, I use:

    make clean && make test

My latest verified results were:

- 73 unit tests passed.
- 0 unit tests failed.
- 2 regression tests passed.

## Scenario Testing

I tested the program using a basic scenario that changes
the temperature, humidity, and switch values over time.

I also tested an invalid scenario containing duplicate
tick numbers. The program correctly rejected the invalid
file and reported the error line.

The regression script compares the program's output
with the expected results to check for mistakes.

## What I Learned

This homework helped me understand how hardware registers
work and how bitwise operations can control switches and LEDs.

I learned that the program needs to read the current LED
register before changing an LED so the other LEDs do not
change accidentally.

I also learned how to organize a C program into separate
files and use functions to make the program easier to
understand.

Another important thing I learned was how to use unit
tests and regression tests to check my program and make
sure changes do not cause problems.

## Cleaning the Project

To remove compiled files, I use:

    make clean
