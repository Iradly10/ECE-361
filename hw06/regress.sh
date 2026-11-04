
#!/bin/sh

# ECE 361 HW06 Regression Tests
# Compare replay output with expected results.

set -eu

echo "Running HW06 regression tests..."

# Create a temporary file for actual output.
actual=$(mktemp)

# Remove temporary file when finished.
trap 'rm -f "$actual"' EXIT

# Run the basic scenario.
./replay scenarios/basic.txt > "$actual"

# Compare actual and expected output.
if diff -u expected/basic.out "$actual"; then
    echo "PASS: basic scenario regression test"
else
    echo "FAIL: basic scenario regression test"
    exit 1
fi

# Verify invalid duplicate ticks are rejected.
if ./replay scenarios/invalid_duplicate_tick.txt \
    > "$actual" 2>&1; then
    echo "FAIL: invalid scenario was accepted"
    exit 1
else
    echo "PASS: invalid duplicate tick rejected"
fi

echo "All regression tests passed."
