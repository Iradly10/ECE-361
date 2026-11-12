#!/bin/sh
set -u
pass=0
fail=0
for input in inputs/*.txt; do
    name=${input##*/}
    actual=$(mktemp)
    ./balance < "$input" > "$actual"
    status=$?
    expected_status=$(cat "expected/$name.status")
    if diff -u "expected/$name" "$actual" && [ "$status" -eq "$expected_status" ]; then
        printf 'PASS regression %s\n' "$name"
        pass=$((pass + 1))
    else
        printf 'FAIL regression %s (exit %s, expected %s)\n' "$name" "$status" "$expected_status"
        fail=$((fail + 1))
    fi
    rm -f "$actual"
done
printf 'Regression summary: %s passed, %s failed\n' "$pass" "$fail"
[ "$fail" -eq 0 ]
