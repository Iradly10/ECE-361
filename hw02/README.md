# ECE 361 — Homework 2

## Overview

This homework reorganizes a single C program into separate modules. The program reads sensor data containing time ticks, temperature, and humidity, then calculates statistics and prints a histogram.

## Modules

- **main.c:** Handles command-line arguments, calls the modules, and prints results.
- **reading.c / reading.h:** Reads sensor data and skips comments, blank lines, and invalid input.
- **stats.c / stats.h:** Calculates minimum, maximum, mean, and the longest run above a threshold. The maximum function uses recursion.
- **histogram.c / histogram.h:** Prints the temperature histogram.

## Module Diagram

```text
                  main.c
                 /  |  \
                v   v   v
          reading stats histogram
```

The main module calls the reading, statistics, and histogram modules. These modules do not call each other.

## Building the Program

Run:

`make`

## Running the Program

Run:

`./readings < data/normal.txt`

To specify a temperature threshold:

`./readings 25 < data/edge.txt`

## Testing

Run:

`make test`

This runs the statistics unit tests and the regression tests to verify that the program's output matches the expected results.

## Cleaning

Run:

`make clean`

This removes the compiled program, object files, and test executable.
