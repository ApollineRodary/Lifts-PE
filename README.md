# Lifts-PE

A C++ codebase for simulating and evaluating the performances of different algorithms for scheduling multiple elevators.

## Context

This project was made by Thibault Hervier, Isaline Plaid, and Apolline Rodary for the Performance Evaluation (PE) course at ENS de Lyon. The goal is to compare state-of-the-art algorithms for elevator scheduling in realistic settings, taking into account rush hours and prioritized floors. In particular, we give a particular focus to reproducing the exact settings of a familiar elevator system (that of the campus where we study).

## Building

To build the project on Unix systems, run the following commands:
```sh
mkdir build
cd build
cmake .. & make
```
This creates executables for all tests.

## Code structure

Simulations are based on a list of `User`s, all with their own goals (which floor to go to at which time), communicating with an `ElevatorSystem` through different commands exposed at every floor. The elevator system controls which `Elevator` is called at which floor and when. Most of the code for elevator systems is left blank, and specific elevator algorithms are implemented by providing an implementation for the virtual member functions of `ElevatorSystem` and `Elevator`.