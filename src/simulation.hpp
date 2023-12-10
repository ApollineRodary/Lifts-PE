#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>

class ElevatorSystem;
class User;

using namespace std;

class Simulation {
private:
    ElevatorSystem& system;
    vector<User*> users;
    int time;
    bool log;

public:
    Simulation(ElevatorSystem& system, vector<User*> users);
    void tick();
    void repeat(int n);
};

#endif