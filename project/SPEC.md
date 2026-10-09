
# ECE 361 - Project 8: Washing Machine

## Project Description

This project is a front-loading washing machine controlled by an embedded system. The washing machine follows a sequence of operations including filling, washing, rinsing, spinning, and draining.

The system uses switches for user input, LEDs to indicate hardware activity, and a temperature and humidity sensor.

## States

The washing machine has six states:

1. IDLE - The washing machine is waiting for START. The door is unlocked.
2. FILL - The inlet valve opens to fill the washing machine with water.
3. WASH - The drum rotates and the heater controls the water temperature.
4. RINSE - The drum rotates to rinse the clothes with fresh water.
5. SPIN - The drum spins quickly to remove water from the clothes.
6. DRAIN - The pump removes water from the washing machine.

## Events

The following events will control the washing machine:

- START_PRESSED - The START switch is pressed while the door is closed.
- DOOR_OPENED - The door-closed switch changes from on to off.
- DOOR_CLOSED - The door-closed switch changes from off to on.
- FILL_COMPLETE - The filling phase reaches 20 ticks.
- WASH_COMPLETE - The washing phase reaches 300 ticks.
- RINSE_COMPLETE - The rinsing phase reaches 120 ticks.
- SPIN_COMPLETE - The spinning phase reaches 60 ticks.
- DRAIN_COMPLETE - The draining phase reaches 30 ticks.

## Temperature Control

During the WASH state:
- The heater turns on when the temperature falls below 38.5 degrees Celsius.
- The heater turns off when the temperature reaches 40.0 degrees Celsius.

## Future Work

The state transition table and additional operating decisions will be developed in Week 5.
