
# Washing Machine Project - Specification

## Project Overview

This project simulates a front-loading washing machine using C programming. The washing machine uses a finite state machine (FSM) to control the different parts of a washing cycle. It reads temperature and humidity sensors, checks switches, controls LEDs, and stores information about the washing process.

The purpose of this project is to practice using state machines, memory-mapped I/O, data structures, and modular programming.

## Washing Machine States

The washing machine has six states:

1. **IDLE** - The washing machine waits for START. The door is unlocked.
2. **FILL** - The washing machine fills with water.
3. **WASH** - The machine washes the clothes and controls the water temperature.
4. **RINSE** - The machine rinses the clothes.
5. **SPIN** - The machine spins the clothes.
6. **DRAIN** - The machine drains the water.

The normal washing sequence is:

IDLE -> FILL -> WASH -> RINSE -> SPIN -> DRAIN -> IDLE

## Events

The state machine uses these events:

- **START** - The START switch is pressed while the door is closed.
- **FILL_COMPLETE** - The filling time has finished.
- **WASH_COMPLETE** - The washing time has finished.
- **RINSE_COMPLETE** - The rinsing time has finished.
- **SPIN_COMPLETE** - The spinning time has finished.
- **DRAIN_COMPLETE** - The draining time has finished.
- **DOOR_OPEN** - The door-closed signal is off during an active cycle.
- **DOOR_CLOSED** - The door-closed signal changes from off to on.

## State Transition Table

The following table shows how the washing machine changes states when an event happens.

| Current State | Event | Next State | Action |
|---|---|---|---|
| IDLE | START | FILL | ACTION_START_FILL |
| FILL | FILL_COMPLETE | WASH | ACTION_START_WASH |
| FILL | DOOR_OPEN | IDLE | ACTION_STOP |
| WASH | WASH_COMPLETE | RINSE | ACTION_START_RINSE |
| WASH | DOOR_OPEN | IDLE | ACTION_STOP |
| RINSE | RINSE_COMPLETE | SPIN | ACTION_START_SPIN |
| RINSE | DOOR_OPEN | IDLE | ACTION_STOP |
| SPIN | SPIN_COMPLETE | DRAIN | ACTION_START_DRAIN |
| SPIN | DOOR_OPEN | IDLE | ACTION_STOP |
| DRAIN | DRAIN_COMPLETE | IDLE | ACTION_FINISH |
| DRAIN | DOOR_OPEN | IDLE | ACTION_STOP |

Any event not listed in the table keeps the washing machine in its current state with ACTION_NONE.

The transition table is implemented in `fsm.c` using an array indexed by the current state and event.

## State Machine Drawing

The normal washing cycle follows this path:

```text
               START
        +----------------+
        |                v
      IDLE ---> FILL ---> WASH
       ^                  |
       |                  v
      DRAIN <--- SPIN <--- RINSE
       |
       +------> IDLE
```

A more detailed view of the normal sequence is:

```text
IDLE
  |
  | START
  v
FILL
  |
  | FILL_COMPLETE
  v
WASH
  |
  | WASH_COMPLETE
  v
RINSE
  |
  | RINSE_COMPLETE
  v
SPIN
  |
  | SPIN_COMPLETE
  v
DRAIN
  |
  | DRAIN_COMPLETE
  v
IDLE
```

If the door opens during FILL, WASH, RINSE, SPIN, or DRAIN, the washing machine stops and returns to IDLE.

## State Durations

Each washing state runs for a specific number of seconds.

| State | Duration |
|---|---|
| FILL | 20 seconds |
| WASH | 300 seconds |
| RINSE | 120 seconds |
| SPIN | 60 seconds |
| DRAIN | 30 seconds |

Each tick represents one second. When the required time has passed, the machine moves to the next state.

## Operating Rules

### Rule 1: Washing Sequence

The washing machine starts in IDLE.

When START is pressed and the door is closed, the machine begins FILL.

After each phase finishes, the machine moves to the next state automatically.

When DRAIN finishes, the machine returns to IDLE.

### Rule 2: State Timing

Each phase lasts for its assigned number of seconds.

The program keeps track of the current tick and the tick when the phase started.

When enough time has passed, the state machine receives a completion event.

### Rule 3: Temperature Control

During WASH, the washing machine monitors the water temperature.

- Below 38.5 degrees Celsius, the heater turns on.
- At 40.0 degrees Celsius or higher, the heater turns off.
- Between 38.5 and 40.0 degrees Celsius, the heater keeps its previous setting.

This prevents the heater from turning on and off repeatedly when the temperature is near the threshold.

If WASH begins with a temperature of 40.0 degrees Celsius or higher, the heater stays off.

## START Switch Behavior

The START switch is only accepted when the washing machine is in IDLE and the door is closed.

START is detected when its switch bit changes from 0 to 1.

If START is pressed while the machine is already running, the program ignores it.

After the washing cycle finishes, another START press can begin a new cycle.

## Door Safety Behavior

The door is unlocked while the machine is in IDLE.

During normal operation, the door is locked while the washing machine is running.

If the door-closed signal turns off during FILL, WASH, RINSE, SPIN, or DRAIN, the machine stops the current cycle and returns to IDLE.

When the door closes again, the machine remains in IDLE until another START press occurs.

This is the safety behavior selected for the simulation.

## Sensor Readings

The washing machine reads two sensor values:

- Temperature in degrees Celsius
- Relative humidity as a percentage

The sensor readings come from the iom361 hardware simulation library.

The raw sensor values are converted using these formulas:

Temperature:

T = (ST / 1048576) * 200 - 50

Humidity:

RH = (SRH / 1048576) * 100

The values are rounded to one decimal place.

The `reading_t` structure contains:

- A timestamp using `time_t`
- Temperature as a float
- Humidity as a float

The `sensor.c` module reads the registers and converts the values. It returns an error if the reading fails.

## Clock and Timestamps

The scenario file provides the starting Unix timestamp.

The program advances time by one second for every tick.

Each reading uses:

timestamp = start_time + tick

The program uses `gmtime()` and `strftime()` to display timestamps in UTC.

This allows the clock to correctly handle changes between minutes, hours, days, and months.

For example:

2027-02-28 23:59:59

becomes:

2027-03-01 00:00:00

### January 2038 Problem

If `time_t` were a signed 32-bit integer, it would reach its maximum value on January 19, 2038, at 03:14:07 UTC.

After that, adding another second could cause an overflow. The timestamp could become negative and represent a date in 1901 instead of 2038.

This could make the washing machine show incorrect timestamps and affect its timing.

Using a 64-bit `time_t` avoids this problem for normal practical use.

## Memory-Mapped I/O

The washing machine communicates with the simulated hardware using memory-mapped registers.

The program reads the sensor and switch registers to determine what is happening.

It writes to the LED and RGB LED registers to show the machine's operating conditions.

The iom361 library simulates these hardware operations.

## LED Outputs

The four regular LEDs represent different washing machine components.

| LED Bit | Name | Meaning |
|---|---|---|
| 0 | INLET | Water inlet valve is open |
| 1 | HEATER | Water heater is on |
| 2 | DRUM | Drum motor is running |
| 3 | PUMP | Drain pump is running |

The LEDs depend on the current washing state.

| State | Active LEDs |
|---|---|
| IDLE | None |
| FILL | INLET |
| WASH | DRUM, and HEATER when needed |
| RINSE | INLET and DRUM |
| SPIN | DRUM |
| DRAIN | PUMP |

## RGB LED Colors

The RGB LED uses a different color for each washing machine state.

| State | Color | RGB Values |
|---|---|---|
| IDLE | Green | (0, 255, 0) |
| FILL | Blue | (0, 0, 255) |
| WASH | Red | (255, 0, 0) |
| RINSE | Cyan | (0, 255, 255) |
| SPIN | Magenta | (255, 0, 255) |
| DRAIN | Yellow | (255, 255, 0) |

These colors make it easier to identify which part of the washing cycle is running.

## Data Storage

The project uses a binary search tree (BST) to store records from the washing machine.

Each record is represented by `record_t`, which contains:

- `reading`: the timestamp, temperature, and humidity
- `state`: the washing machine state
- `status`: the packed status word

The tree stores records using two keys:

1. Washing machine state
2. Timestamp

The `record_compare()` function compares the states first.

If the states are equal, it compares the timestamps.

This determines where each record belongs in the tree.

### In-Order Traversal

An in-order traversal visits the left subtree, the current node, and then the right subtree.

Since the tree is organized by state and timestamp, an in-order traversal produces records grouped by state and ordered by timestamp within each state.

It does not necessarily show the entire washing cycle in chronological order because state is compared before timestamp.

The BST supports creating, inserting, finding, counting, checking height, traversing, and destroying records.

## Event History Stack

The project uses a stack to keep track of state changes.

Each event stores:

- The tick when the state changed
- The new washing machine state

The stack follows last-in, first-out (LIFO) order.

This means the newest state change is removed and displayed first.

### Stack Growth

The stack starts with enough room for eight events.

When the stack becomes full, its capacity doubles.

For example:

8 -> 16 -> 32 -> 64

Doubling the capacity means the program does not need to resize the stack every time a new event is added.

It also allows the stack to handle larger event histories without knowing the total number of events ahead of time.

### Stack Comparison

My project uses a dynamic array stack.

Another option would be a linked-list stack.

| Operation | Dynamic Array Stack | Linked-List Stack |
|---|---|---|
| Push | O(1) amortized | O(1) |
| Pop | O(1) | O(1) |
| Peek | O(1) | O(1) |
| Count | O(1) | O(1), with a stored count |

The dynamic array stack can occasionally take O(n) time when it needs to resize and copy its elements.

A linked-list stack can add new elements without resizing, but it needs extra memory for pointers and usually requires a separate allocation for each new node.

I chose the dynamic array because it is simple to manage and works well for this project.

## Report Mode

The washing machine includes a report mode that finds the highest water temperatures recorded during WASH.

The report uses a top-k method, with k set to 3 in the main program.

The report follows these rules:

1. Only readings recorded during WASH are included.
2. The readings are sorted from highest temperature to lowest.
3. If two readings have the same temperature, the earlier timestamp comes first.
4. If fewer than k readings exist, all available readings are reported.
5. If no WASH readings exist, the report displays a message saying there are no readings.

The value of k is a parameter, so it can be changed without rewriting the report algorithm.

The report reads the stored records using the functions provided by `store.h`.

The report can be generated with:

./washer scenarios/report.txt --report

## Status Word

The washing machine stores its state and LED information in one 32-bit unsigned integer.

The status word uses this layout:

| Bits | Information |
|---|---|
| 0-2 | Washing machine state |
| 3 | INLET LED |
| 4 | HEATER LED |
| 5 | DRUM LED |
| 6 | PUMP LED |
| 7 | Door locked |
| 8-31 | Unused, always 0 |

The state uses three bits because there are six possible washing machine states.

The door-locked bit is 1 whenever the machine is in FILL, WASH, RINSE, SPIN, or DRAIN.

The door-locked bit is 0 when the machine is in IDLE.

The status word combines several values into a single integer.

The current `main.c` packs the status fields using bit masks and shifts. The assignment also requires the HW1 `get_field()` and `set_field()` functions for packing and unpacking. That implementation still needs to be added and tested.

## Event Generation

The program creates events inside `main.c`.

It reads the switches, checks whether the door is closed, detects a new START press, and checks whether the current phase has finished.

Based on those conditions, it chooses an event and passes it to the FSM.

The FSM uses its transition table to determine the next state.

I kept event detection in `main.c` because it already handles the hardware inputs and time. This keeps `fsm.c` focused on the transition table and makes the state machine easier to test separately.

## Scenario Testing

The project uses scenario files to simulate different operating conditions.

The scenarios include:

1. **Normal Operation** - A complete washing cycle returns to IDLE.
2. **Stack Growth** - The event history contains more than eight events.
3. **Temperature Thresholds** - The heater is tested at and around its thresholds.
4. **Temperature Control** - The heater responds as the temperature changes.
5. **Unexpected Input** - START and door-switch changes are tested during operation.
6. **Report Testing** - The program processes multiple readings and generates a temperature report.
7. **Midnight Rollover** - The clock crosses midnight and changes to the next date.

An additional scenario checks the change from February 28, 2027, to March 1, 2027.

These scenarios help verify that the program follows its operating rules.

## Design Decisions

Some behaviors were not specifically defined in the assignment, so the following decisions were made:

- START is ignored during an active washing cycle.
- An unexpected door-open signal stops the cycle and returns the machine to IDLE.
- Closing the door again does not automatically restart the cycle.
- The heater starts off when entering WASH and follows the temperature thresholds.
- A new washing cycle requires a new START press after returning to IDLE.
- Temperature thresholds use the rounded sensor readings.
- Events are generated in `main.c`, while transitions are handled in `fsm.c`.

These decisions make the washing machine's behavior more predictable.

## Evaluation of the Given States

I think the six states provided for the washing machine work well because each one represents a different part of the washing cycle. It makes the program easier to organize because each state has its own purpose and duration. One limitation is that there is no separate PAUSE or ERROR state. For example, if the door opens during operation, my program returns directly to IDLE instead of pausing the cycle. Adding a separate error or pause state could make the simulation more realistic, but the six given states are enough for this project.

## Conclusion

This specification explains how the washing machine is expected to operate.

The finite state machine controls the washing sequence, the sensors provide measurements, and the program uses different data structures to store records and event history.

The scenario tests help check that the washing machine follows its rules and responds correctly to different inputs.
