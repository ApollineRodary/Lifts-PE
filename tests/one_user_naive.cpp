#include <iostream>
#include <iomanip>
#include <string>

#include "user.hpp"
#include "elevator.hpp"
#include "elevator_system.hpp"
#include "simulation.hpp"
#include "log.hpp"

using namespace std;

class TestElevatorSystem: public ElevatorSystem {
public:
    TestElevatorSystem(int min_floor, int max_floor): ElevatorSystem(min_floor, max_floor) {};

    void call(int floor, int time, string command) override {
        debugStream("TestElevatorSystem::call") << '!' << time << '?' << command << '!' << floor << endl;
        for (auto elevator: getElevators())
            elevator->moveToFloor(floor);
    }
};

class TestElevator: public Elevator {
public:
    TestElevator(ElevatorSystem& system, int floor): Elevator(system, floor) {};

    void requestFloor(int floor) override {
        debugStream("TestElevator::requestFloor") << '!' << floor << endl;
        moveToFloor(floor);
    }
};

int main() {
    // One single elevator, starting at floor 2
    TestElevatorSystem system(0, 4);
    TestElevator e(system, 2);
    
    // One single user, going from floor 0 to floor 3
    User user(system);
    Goal g;
    g.source_floor = 0;
    g.target_floor = 3;
    g.time = 5;
    user.addGoal(g);

    vector<User*> users;
    users.push_back(&user);

    Simulation sim(system, users);
    sim.repeat(100);
}