#include <iostream>
#include <string>

#include "user.hpp"
#include "elevator.hpp"
#include "elevator_system.hpp"
#include "simulation.hpp"
#include "log.hpp"
#include "algorithms/scan.cpp"

int main() {
    /**
     * Testing the SCAN elevator algorithm on a single elevator
     * Apolline goes from floor 0 to floor 2
     * Isaline then requests to go from floor -1 to floor 2 while Apolline is in the elevator
     * Thibault then requests to go from floor 4 to floor 3 while Apolline is in the elevator
     * SCAN should prioritize Thibault
     */

    ScanElevatorSystem monod(-1, 4);
    ScanElevator e(monod, 2, 13);

    User apo(monod, 0, 1);
    User tib(monod, 4, 1);
    User isa(monod, -1, 1);

    apo.addGoal(Goal(0, 2, 6));
    isa.addGoal(Goal(-1, 2, 23));
    tib.addGoal(Goal(4, 3, 25));

    Simulation sim(monod, {&apo, &tib, &isa});
    sim.repeat(100);
}