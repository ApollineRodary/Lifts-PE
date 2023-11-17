#include <vector>
#include <iostream>
#include "simulation.hpp"
#include "elevator_system.hpp"
#include "user.hpp"
#include "log.hpp"

using namespace std;

Simulation::Simulation(ElevatorSystem& system, vector<User*> users) : system(system), users(users) {
    time = 0;
}

void Simulation::tick() {
    for (auto user: users)
        user->tick(time);
    time++;
}

void Simulation::repeat(int n) {
    for (int i=0; i<n; i++) {
        if (i==0)
            debugStream("Simulation::repeat") << '!' << i << endl;
        tick();
    }
}