# ECE 361 — Homework 8: Binary Search Tree ADT

## Overview
This assignment ports the provided TutorialsPoint `BinarySearchTree.c` example into a reusable BST ADT. The tree is owned by a `bst_t *` instead of a global variable, and search does not print. Equal keys are inserted into the right subtree. The optional `bst_remove` bonus is included.

## Build and test
From the `hw08` directory:

```sh
make
make test
make memcheck
make bonus
make clean
```

`make` builds `treestats`. `make test` runs BST unit tests and input/output smoke tests. `make memcheck` builds and runs with AddressSanitizer and UndefinedBehaviorSanitizer (a sanitizer-based memory check, not Valgrind). `make bonus` runs the optional removal tests with sanitizers.

Try:

```sh
printf '27 14 35 10 19 31 42\n' | ./treestats
# n 7, height 3
printf '' | ./treestats
# empty tree
printf '1 2 3 4 nope\n' | ./treestats
# not a number after 4 values  (exit status 1)
```

## Original crash: reproduce before fixing
The regression test is `original crash: search empty tree` in `tests/test_bst.c`: create an empty tree and search for 31. I temporarily used the original loop condition, `while (current->data != key)`, in the port to reproduce the failure, then changed it to check `current != NULL` before accessing `current->data`.

First line of the sanitizer report from the intentionally unfixed port:

```text
/tmp/hw08_buggy.c:75:19: runtime error: member access within null pointer of type 'const struct node'
```

The original example's `main` inserted seven keys before searching, so it never tested a search on an empty tree. The final `bst_contains` handles empty and NULL trees safely and counts node comparisons, including the matching node.

## Required tests
`tests/test_bst.c` covers the empty-tree crash, NULL arguments, the seven original keys (size 7, height 3), finding 31 in 3 visits, missing 15 in 3 visits, duplicates retained, increasing-order height equal to size, and destroying a populated tree. Each check prints PASS or FAIL; the test executable returns nonzero on any failure.

## Insertion order experiment
Commands executed:

```sh
seq 1 1023 | ./treestats
# n 1023, height 1023
seq 1 1023 | sort -R | ./treestats
# n 1023, height 20
```

In increasing order, every new key goes to the right, so the tree becomes a chain of height 1023; the shuffled run gave a much shorter tree of height 20 (the exact shuffled height varies each run). Since a search follows one path down the tree, a taller tree can require more node visits, up to 1023 here instead of at most 20 in that shuffled tree.

## Files
- `bst.h`: public opaque ADT interface and function contracts
- `bst.c`: implementation, with original `insert` and `search` ancestry noted in comments
- `treestats.c`: reads integers from stdin and prints exact requested output
- `tests/test_bst.c`: required regression and behavior tests
- `tests/test_remove.c`: bonus removal tests
- `BONUS.md`: bonus diagrams and cases
- `BinarySearchTree_original.c`: unchanged starter code for comparison
- `AI_USAGE.md`: AI assistance disclosure

## Test environment
Compiled with a C11 compiler and `-Wall -Wextra -Wpedantic -Werror`. `make test`, `make memcheck`, and `make bonus` passed in the development environment. Re-run on your own Mac or course machine before submitting.
