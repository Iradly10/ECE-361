# ECE 361 - Homework 7

## Overview

For Homework 7, I worked with dynamic memory in C by creating a stack that stores integers. I learned how `malloc`, `realloc`, and `free` are used to allocate, resize, and release memory.

I also worked on a bracket checker that uses the stack to determine whether brackets are balanced. This assignment helped me understand how stacks work, how dynamic memory is managed, and how to identify memory errors.

## Part 1 - Dynamic Stack

For Part 1, I worked with a dynamic integer stack using `istack.h` and `istack.c`.

The stack starts with a capacity of 8 integers. When the stack becomes full, its capacity doubles so that more values can be stored.

The stack uses:

- `malloc` to allocate memory.
- `realloc` to increase the stack capacity.
- `free` to release memory when the stack is destroyed.

The stack supports creating and destroying a stack, pushing and popping values, peeking at the top value, checking whether the stack is empty, getting its count and capacity, and copying its contents into an array.

One thing I learned was that using a temporary pointer with `realloc` is important. If resizing fails, the original memory allocation should not be lost.

I also learned that the stack should not shrink when values are removed.

## Part 2 - Bracket Checker

For Part 2, I worked on `balance.c`, which reads characters from standard input and uses the stack to check matching brackets.

The program checks three types of brackets:

- Parentheses: `()`
- Square brackets: `[]`
- Curly brackets: `{}`

The program detects correctly balanced brackets, mismatched closing brackets, closing brackets without an opening bracket, and opening brackets that are never closed.

It also counts the number of matching bracket pairs and keeps track of the deepest number of open brackets.

To run the bracket checker:

```bash
./balance < inputs/balanced.txt
```

## Part 3 - Building and Testing

I used the terminal in VS Code to compile and test my homework.

### Build Commands

```bash
make clean
make
make test
make memcheck
```

### Unit Test Results

I ran the stack unit tests to check the different stack operations.

```text
Summary: 27 passed, 0 failed
```

All 27 unit tests passed.

The tests checked empty stacks, NULL pointers, pushing and popping values, resizing, capacity, copying values into arrays, and using multiple stacks.

### Regression Test Results

I also ran the bracket checker regression tests.

```text
PASS regression balanced.txt
PASS regression deep.txt
PASS regression empty.txt
PASS regression mismatch.txt
PASS regression nothing_open.txt
PASS regression unclosed.txt
Regression summary: 6 passed, 0 failed
```

All 6 regression tests passed.

### Memory Sanitizer Results

I ran `make memcheck` using AddressSanitizer and UndefinedBehaviorSanitizer.

```text
Summary: 27 passed, 0 failed
Regression summary: 6 passed, 0 failed
```

All tests passed without sanitizer errors during normal testing.

Because I was using a Mac, I used `ASAN_OPTIONS=detect_leaks=0` because AddressSanitizer's leak detection option was not supported in my environment.

### Memory Leak Check

I also used Apple's `leaks` tool to check for memory leaks in the stack tests.

Command:

```bash
leaks --atExit -- ./tests/test_istack
```

Result:

```text
Process 97968: 0 leaks for 0 total leaked bytes.
```

This showed that no memory leaks were detected during the stack unit tests.

## Part 4 - Memory Bug Experiments

For Part 4, I intentionally tested different memory mistakes to see how the sanitizer and memory leak checker would detect them.

After completing the experiments, I restored the original code and ran the tests again.

### Experiment 1 - Destroying the Same Stack Twice

For the first experiment, I created a temporary test program that called `istack_destroy` twice on the same stack.

Command:

```bash
ASAN_OPTIONS=detect_leaks=0 ./double_free_test
```

**First diagnostic line:**

```text
==99039==ERROR: AddressSanitizer: heap-use-after-free on address 0x603000001b40
```

**Sanitizer summary:**

```text
SUMMARY: AddressSanitizer: heap-use-after-free istack.c:17 in istack_destroy
```

**Explanation:**

I tested what happens when I destroy the same stack twice. AddressSanitizer detected a heap-use-after-free error because the program tried to access memory that had already been freed.

This helped me understand why a stack should only be destroyed once and why accessing freed memory can cause problems.

### Experiment 2 - Pushing After Destroying a Stack

For the second experiment, I created another temporary test program. This time, I destroyed a stack and then tried to push another value onto it.

Command:

```bash
ASAN_OPTIONS=detect_leaks=0 ./use_after_free_test
```

**First diagnostic line:**

```text
==784==ERROR: AddressSanitizer: heap-use-after-free on address 0x603000001b48
```

**Sanitizer summary:**

```text
SUMMARY: AddressSanitizer: heap-use-after-free istack.c:22 in istack_push
```

**Explanation:**

I tested what happens when I try to push a value onto a stack after destroying it.

AddressSanitizer detected a heap-use-after-free error because the program tried to access memory that had already been released.

This helped me understand why I should not continue using a stack pointer after destroying the stack.

### Experiment 3 - Memory Leak

For the third experiment, I temporarily removed `istack_destroy(stack)` from the successful execution path in `balance.c`.

I then ran the bracket checker using Apple's `leaks` tool.

Command:

```bash
leaks --atExit -- ./balance < inputs/balanced.txt
```

**Memory leak report:**

```text
Process 1520: 2 leaks for 80 total leaked bytes.
```

**Explanation:**

I tested what happens when I leave out `istack_destroy` in the bracket checker.

The memory leak checker detected 2 leaks totaling 80 bytes.

This happened because the stack structure and its internal array were not freed before the program finished.

The stack structure used 32 bytes, and the internal array used 48 bytes.

This experiment helped me understand why dynamically allocated memory needs to be freed when it is no longer needed.

### Restoring the Original Code

After completing the three memory experiments, I restored the original `balance.c` file.

I then ran:

```bash
make clean
make test
make memcheck
```

The final results were:

```text
Summary: 27 passed, 0 failed
Regression summary: 6 passed, 0 failed
```

The unit tests and regression tests passed again, including the sanitizer-enabled tests.

I also removed the temporary experiment programs, backup files, and generated debugging folders.

## AI Usage

I used ChatGPT to help me understand the assignment requirements and prepare the C implementation, tests, and documentation. It also helped me troubleshoot build errors and understand the memory sanitizer results.

I ran the programs and tests in VS Code to verify their behavior.

More information about my AI usage is included in `AI_USAGE.md`.