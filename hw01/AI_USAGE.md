# How AI was Used

##

This is how I used AI:

  - Built tests
  - Guidance on terminal and git commands
  - Understand bit manipulation and bit fields.
  - Guidance to develop the functions in `bits.c`.
  - Guidance on the Makefile.
  - Debug compiler errors.

- One issue I had to fix:
  - While working on `status.c`, some function calls were accidentally placed outside of the `status_unpack()` function.
  - This caused compiler errors involving `get_field()` and `sign_extend()`.
  - I found the problem by compiling the code with `-Wall` and `-Wextra` and reading the compiler error messages.
  - I fixed the problem by moving the statements inside `status_unpack()` and recompiling.
  - After the fix, the file compiled without errors.