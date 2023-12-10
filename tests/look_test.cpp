#include <iostream>
#include <string>

#include "user.hpp"
#include "elevator.hpp"
#include "elevator_system.hpp"
#include "simulation.hpp"
#include "log.hpp"
#include "algorithms/look.cpp"
#include "user_presets/pe_group.hpp"

int main() {
    /**
     * Testing the LOOK elevator algorithm on a single elevator
     * Apolline goes from floor 0 to floor 2
     * Isaline then requests to go from floor -1 to floor 2 while Apolline is in the elevator
     * Thibault then requests to go from floor 4 to floor 3 while Apolline is in the elevator
     * LOOK should prioritize Thibault
     */

    LookElevatorSystem monod(-1, 4);
    LookElevator e(monod, 2, 13);

    PEGroup users(monod);
    Simulation sim(monod, users());
    sim.repeat(100);

    cout << "\nWaiting times:";
    for (int w: sim.getWaitingTimes())
        cout << " " << w;
    cout << "\nRegrets:";
    for (int r: sim.getRegretTimes())
        cout << " " << r;
    cout << endl;

    return 0;
}