# Lifts-PE

A C++ codebase for simulating and evaluating the performances of different algorithms for scheduling multiple elevators.

## Context

This project was made by Thibault Hervier, Isaline Plaid, and Apolline Rodary for the Performance Evaluation (PE) course at ENS de Lyon. The goal is to compare state-of-the-art algorithms for elevator scheduling in realistic settings, taking into account rush hours and prioritized floors. In particular, we give a particular focus to reproducing the exact settings of a familiar elevator system (that of the campus where we study).

## Building

To build the project on Unix systems, run the following commands:
```sh
mkdir build
cd build
cmake .. && make
```
This creates executables for all tests.

## Code structure

Simulations are based on a list of `User`s, all with their own goals (which floor to go to at which time), communicating with an `ElevatorSystem` through different commands exposed at every floor. The elevator system controls which `Elevator` is called at which floor and when. Most of the code for elevator systems is left blank, and specific elevator algorithms are implemented by providing an implementation for the virtual member functions of `ElevatorSystem` and `Elevator`.

## Tests

### Model verification

In order to verify the model, build the project then execute `one_source_multi_targ` and `one_source_multi_targ_two` in the `build` folder. The results will be saved respectively in `results/one_src_multiple_target.json` and `results/one_src_multiple_target_two_elevators.json`.  
Then execute the Python script `model.py`, that will plot the graphs and save the figure in `results/graphs_one_source_multiple_targets.png`.

### Comparison of LOOK and SCAN on "realistic" examples

Build the project and execute `realistic_look` and `realistic_scan`. Results will be stored in `results/realistic_look.txt` and `results/realistic_scan.txt` respectively.  
Do the same thing for `realistic_look_two` and `realistic_scan_two` to use two elevators.  
By a very quick empiric verification, we have that the elevator is stopped at a given floor for approxmiatively 13s before it starts to move, and that it takes approxmiatively 2s for it to go from one floor to another.