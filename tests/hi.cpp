#include <iostream>
#include <iomanip>
#include <string>

#include "user.hpp"
#include "elevator_system.hpp"
#include "simulation.hpp"
#include "log.hpp"

using namespace std;

class TestElevatorSystem: public ElevatorSystem {
public:
    TestElevatorSystem(int min_floor, int max_floor): ElevatorSystem(min_floor, max_floor) {};

    void call(int floor, string command) override {
        debugStream("TestElevatorSystem::call") << '!' << floor << '?' << command << endl;
    }
};

int main() {
    TestElevatorSystem system(0, 4);
    vector<User*> users;
    User user(system);
    Goal g;
    g.source_floor = 0;
    g.target_floor = 3;
    g.time = 5;
    user.addGoal(g);
    users.push_back(&user);
    Simulation sim(system, users);

    sim.repeat(100);
}