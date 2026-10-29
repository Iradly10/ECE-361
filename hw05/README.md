
# ECE 361 - Homework 5
## Combination Lock Finite State Machine

### Description
For this homework, I created a combination lock using a finite state machine (FSM) in C. The combination is 3-1-4. The program uses a transition table to determine the next state and action based on the input.

If the correct digits are entered, the door unlocks. If a wrong digit is entered, the lock beeps and resets. The program also handles closing the door and timer timeouts.

### Files
- lock.h - Contains the enums, structure, and function declarations.
- lock.c - Contains the transition table and functions.
- main.c - Reads the input and displays the lock's behavior.
- data/codes.txt - Contains sample combinations.
- tests/test_lock.c - Tests the lock and its transitions.
- Makefile - Builds and tests the program.
- AI_USAGE.md - Explains how AI was used.

### States
- LOCKED - The door is locked and waiting for 3.
- GOT1 - The first digit (3) was entered correctly.
- GOT2 - The first two digits (3 and 1) were entered correctly.
- OPEN - The full combination was entered and the door is unlocked.

### Events
- E_RIGHT - The correct next digit was entered.
- E_WRONG - An incorrect digit was entered.
- E_CLOSE - The door was closed.
- E_TIMEOUT - The timer expired.

### Actions
- A_NONE - No action is performed.
- A_UNLOCK - Unlocks the door.
- A_LOCK - Locks the door.
- A_BEEP - Beeps when a wrong digit is entered.

### State Diagram

```text
LOCKED --3--> GOT1 --1--> GOT2 --4--> OPEN
  ^             |             |           |
  |             |             |           |
  +-- WRONG ----+-- WRONG ----+           |
  |                                       |
  +----------- CLOSE / TIMEOUT -----------+

Wrong digits return to LOCKED with BEEP.
Timeout during entry returns to LOCKED silently.
Closing during entry does not change the state.
While OPEN, digits are ignored.
Closing or timing out while OPEN returns to LOCKED.
```

### Transition Table

| Current State | Event | Next State | Action |
|---|---|---|---|
| LOCKED | RIGHT | GOT1 | NONE |
| LOCKED | WRONG | LOCKED | BEEP |
| LOCKED | CLOSE | LOCKED | NONE |
| LOCKED | TIMEOUT | LOCKED | NONE |
| GOT1 | RIGHT | GOT2 | NONE |
| GOT1 | WRONG | LOCKED | BEEP |
| GOT1 | CLOSE | GOT1 | NONE |
| GOT1 | TIMEOUT | LOCKED | NONE |
| GOT2 | RIGHT | OPEN | UNLOCK |
| GOT2 | WRONG | LOCKED | BEEP |
| GOT2 | CLOSE | GOT2 | NONE |
| GOT2 | TIMEOUT | LOCKED | NONE |
| OPEN | RIGHT | OPEN | NONE |
| OPEN | WRONG | OPEN | NONE |
| OPEN | CLOSE | LOCKED | LOCK |
| OPEN | TIMEOUT | LOCKED | LOCK |

### How to Build

Open the terminal inside the hw05 folder and type:

```bash
make
```

This compiles the program and creates the lock executable.

### How to Run

To test the correct combination:

```bash
echo "314" | ./lock
```

Expected output:

```text
LOCKED 3 RIGHT -> GOT1 NONE
GOT1 1 RIGHT -> GOT2 NONE
GOT2 4 RIGHT -> OPEN UNLOCK
final: OPEN
```

To run the sample data file:

```bash
./lock < data/codes.txt
```

### How to Test

Run:

```bash
make test
```

The test program checks all 16 transitions, input events, and different combinations.

Test results:

```text
Tests run: 32
Tests passed: 32
Tests failed: 0
```

### How to Clean

Run:

```bash
make clean
```

This removes the compiled files without deleting the source code.

### What I Learned

This homework helped me understand how a finite state machine works in C. I learned how to use a transition table instead of writing a long chain of if statements for changing states. I also practiced testing different inputs and using a Makefile to compile and run my program.
