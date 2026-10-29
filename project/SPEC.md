
# Washing Machine Project - Specification

## Project Overview

This project simulates a front-loading washing machine using C programming. The washing machine uses a finite state machine (FSM) to control the different parts of a washing cycle. It also reads temperature and humidity sensors, checks switches, controls LEDs, and stores information about the washing process.

The purpose of this project is to practice using state machines, memory-mapped I/O, data structures, and modular programming.

## Washing Machine States

The washing machine has six states:

1. **IDLE** - The washing machine is waiting for START. The door is unlocked.
2. **FILL** - The washing machine is filling with water.
3. **WASH** - The washing machine is washing the clothes and controlling the water temperature.
4. **RINSE** - The washing machine is rinsing the clothes.
5. **SPIN** - The washing machine is spinning the clothes.
6. **DRAIN** - The washing machine is draining the water.

The normal washing sequence is:

IDLE -> FILL -> WASH -> RINSE -> SPIN -> DRAIN -> IDLE

## Events

The state machine responds to the following events:

- **START** - The START switch is pressed while the door is closed.
- **FILL_COMPLETE** - The filling time has finished.
- **WASH_COMPLETE** - The washing time has finished.
- **RINSE_COMPLETE** - The rinsing time has finished.
- **SPIN_COMPLETE** - The spinning time has finished.
- **DRAIN_COMPLETE** - The draining time has finished.
- **DOOR_OPEN** - The door-closed signal is off during an active cycle.
- **DOOR_CLOSED** - The door-closed signal changes from off to on.

## State Durations

Each washing state runs for a specific number of seconds.

| State | Duration |
|---|---|
| FILL | 20 seconds |
| WASH | 300 seconds |
| RINSE | 120 seconds |
| SPIN | 60 seconds |
| DRAIN | 30 seconds |

After DRAIN finishes, the washing machine returns to IDLE.

## Operating Rules

### Rule 1: Washing Sequence

The washing machine starts in IDLE.

When START is pressed and the door is closed, the machine begins FILL.

After each phase finishes, the machine automatically moves to the next state.

When DRAIN finishes, the machine returns to IDLE.

### Rule 2: State Timing

Each phase lasts for its assigned number of seconds.

The program keeps track of time using ticks. Each tick represents one second.

When the required time has passed, the state machine moves to the next phase.

### Rule 3: Temperature Control

During WASH, the washing machine monitors the water temperature.

- If the temperature is below 38.5 degrees Celsius, the heater turns on.
- If the temperature reaches 40.0 degrees Celsius or higher, the heater turns off.
- If the temperature is between 38.5 and 40.0 degrees Celsius, the heater keeps its previous setting.

This prevents the heater from switching on and off repeatedly when the temperature is near the threshold.

If WASH begins with a temperature of 40.0 degrees Celsius or higher, the heater stays off.

## START Switch Behavior

The START switch is only accepted when the washing machine is in IDLE and the door is closed.

If START is pressed while the machine is already running, the program ignores the press.

After a washing cycle finishes, a new START press can begin another cycle.

## Door Safety Behavior

The door is unlocked while the machine is in IDLE.

During normal operation, the door is locked while the washing machine is running.

If the door-closed signal turns off during FILL, WASH, RINSE, SPIN, or DRAIN, the machine stops the current cycle and returns to IDLE.

This is the safety behavior selected for this simulation.

When the door is closed again, the machine remains in IDLE until START is pressed.

## Sensor Readings

The washing machine reads two sensor values:

- Temperature in degrees Celsius
- Relative humidity as a percentage

The sensor readings are obtained using the iom361 hardware simulation library.

The program converts raw sensor values into readable measurements.

Sensor readings are also saved with timestamps so the washing machine can keep a history of its operation.

## Clock and Timestamps

The scenario file provides the starting Unix timestamp.

The program advances time by one second for every tick.

Each stored reading uses the starting timestamp plus the current tick.

The clock must correctly handle changes between minutes, hours, days, and months.

For example, the time after 23:59:59 is 00:00:00 on the following day.

## Memory-Mapped I/O

The washing machine communicates with the simulated hardware using memory-mapped registers.

The program reads the sensor and switch registers to determine what is happening.

It also writes to the output registers to control the LEDs and RGB LED.

These outputs represent the current operating conditions of the washing machine.

## Data Storage

The project uses a binary search tree to store records of the washing machine's operation.

Each record contains:

- A sensor reading
- A timestamp
- The washing machine state
- A status value

Records are organized by state and timestamp.

The binary search tree supports inserting, finding, counting, and visiting stored records.

## Event History

The project uses a stack to keep track of state changes.

When the washing machine changes states, the event is saved in the stack.

The stack follows last-in, first-out (LIFO) order.

This means the most recent state change is displayed first when viewing the event history.

The stack can grow when more events are added.

## Report Mode

The washing machine includes a report mode.

The report focuses on the highest temperature readings recorded during WASH.

The report selects the top readings and displays their information.

If two readings have the same temperature, the earlier timestamp is given priority.

This allows the program to summarize temperature measurements collected during the washing cycle.

## Scenario Testing

The project uses scenario files to simulate different operating conditions.

The planned tests include:

1. **Normal Operation** - The washing machine completes a full cycle and returns to IDLE.
2. **Stack Growth** - The event history contains more than eight events.
3. **Temperature Thresholds** - The heater responds correctly at and around its temperature thresholds.
4. **Temperature Control** - The heater turns on, turns off, and turns on again as the temperature changes.
5. **Unexpected Input** - START and door-switch changes are tested during different states.
6. **Report Testing** - The program processes multiple readings and generates a temperature report.
7. **Midnight Rollover** - The simulated clock correctly changes to the next date.

The scenarios help verify that the program follows the operating rules and handles different inputs.

## Design Decisions

Some behaviors were not specifically defined in the assignment, so the following decisions were made:

- START is ignored during an active washing cycle.
- An unexpected door-open signal stops the cycle and returns the machine to IDLE.
- Closing the door again does not automatically restart the cycle.
- The heater starts off when entering WASH and follows the temperature thresholds.
- A new washing cycle requires a new START press after returning to IDLE.

These decisions are included so the program has predictable behavior when unexpected situations occur.

## Conclusion

This specification explains how the washing machine is expected to operate. The state machine controls the washing sequence, the sensors provide measurements, and the program uses different data structures to keep track of information.

The scenario tests are used to check that the program follows these rules and responds correctly to different situations.
