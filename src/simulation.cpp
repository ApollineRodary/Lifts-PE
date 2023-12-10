#include <vector>
#include <iostream>
#include "simulation.hpp"
#include "elevator_system.hpp"
#include "user.hpp"
#include "elevator.hpp"
#include "log.hpp"

using namespace std;

Simulation::Simulation(ElevatorSystem& system, vector<User*> users) : system(system), users(users) {
    time = 0;
}

void Simulation::tick() {
    system.tick(time);
    for (auto user: users)
        user->tick(time);
    for (auto elevator: system.getElevators())
        elevator->tick(time);
    time++;
}

void Simulation::repeat(int n) {
    debugStream("Simulation::repeat") << '!' << "" << '?' << "Starting simulation" << endl;
    debugStream("Simulation::repeat") << '!' << users.size() << '?' << "users" << '!' << system.getElevators().size() << '?' << "elevators" << endl;
    for (int i=0; i<n; i++)
        tick();
}

vector<int> Simulation::getRegretTimes() {
    vector<int> v = {};
    for (auto u: users)
        v.push_back(u->getTotalRegretTime());
    return v;
}

vector<int> Simulation::getWaitingTimes() {
    vector<int> v = {};
    for (auto u: users)
        v.push_back(u->getTotalWaitingTime());
    return v;
}