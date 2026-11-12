# ECE 361 - Homework 7 AI Usage

## AI Tool Used

I used ChatGPT to help me understand Homework 7 and work through the different parts of the assignment.

## How I Used AI

- ChatGPT helped me break down the assignment instructions so I could understand what each part required.
- I received help writing and reviewing the C code for the dynamic stack and bracket checker.
- ChatGPT explained how `malloc`, `realloc`, and `free` work and why they are important when managing memory.
- I learned how a stack uses push, pop, and peek operations to store and remove values.
- ChatGPT helped me understand why the stack starts with a capacity of 8 and doubles when more space is needed.
- I received help creating and understanding the unit tests, regression tests, and Makefile.
- ChatGPT explained how AddressSanitizer and UndefinedBehaviorSanitizer can help detect memory errors.
- I received guidance on performing the three memory experiments and understanding the errors they produced.
- ChatGPT helped me troubleshoot testing problems on my Mac and understand how to use Apple's `leaks` tool.

## What I Learned

This assignment helped me better understand how dynamic memory works in C. I learned why memory needs to be allocated and freed correctly and how a stack can grow when it becomes full.

I also learned how a stack can be used to check matching brackets and why it is important to test different situations, including incorrect inputs and memory errors.

## Verification

I used the terminal in VS Code to compile and test my code.

I ran the following commands:

```bash
make clean
make
make test
make memcheck
```

My final results were:

- 27 unit tests passed with 0 failures.
- 6 regression tests passed with 0 failures.
- AddressSanitizer and UndefinedBehaviorSanitizer completed without errors during normal testing.
- Apple's `leaks` tool reported 0 leaks during the stack unit tests.

I also completed the three memory experiments to see what happens when memory is used incorrectly. After each experiment, I reviewed the error messages to understand what caused the problem.

After finishing the experiments, I restored the original code and ran the tests again to make sure everything still worked correctly.

## Final Summary

ChatGPT helped me understand the assignment, prepare and review the code, and learn how to test for memory errors. I used the explanations to better understand what the functions were doing and how the program worked.

I ran the tests in VS Code and checked the results before uploading the completed homework to GitHub.