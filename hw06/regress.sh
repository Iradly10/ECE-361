#!/bin/sh

# ECE 361 - Homework 6 Regression Tests

set -eu

echo "Running HW06 regression tests..."

actual=$(mktemp)
trap 'rm -f "$actual"' EXIT

# Test the basic scenario.
./replay scenarios/basic.txt > "$actual"

if diff -u expected/basic.out "$actual"; then
    echo "PASS: basic scenario regression test"
else
    echo "FAIL: basic scenario regression test"
    exit 1
fi

# Test the multiple switches scenario.
./replay scenarios/multiple_switches.txt > "$actual"

if diff -u expected/multiple_switches.out "$actual"; then
    echo "PASS: multiple switches regression test"
else
    echo "FAIL: multiple switches regression test"
    exit 1
fi

# Test invalid duplicate ticks.
if ./replay scenarios/invalid_duplicate_tick.txt > "$actual" 2>&1; then
    echo "FAIL: invalid duplicate tick was accepted"
    exit 1
else
    echo "PASS: invalid duplicate tick rejected"
fi

# Test missing start timestamp.
if ./replay scenarios/invalid_missing_start.txt > "$actual" 2>&1; then
    echo "FAIL: missing start timestamp was accepted"
    exit 1
else
    echo "PASS: missing start timestamp rejected"
fi

# Test missing scenario field.
if ./replay scenarios/invalid_missing_field.txt > "$actual" 2>&1; then
    echo "FAIL: missing scenario field was accepted"
    exit 1
else
    echo "PASS: missing scenario field rejected"
fi

echo "All regression tests passed."
