#ifndef ELEVATOR_SYSTEM_HPP
#define ELEVATOR_SYSTEM_HPP

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Elevator;
class User;

class ElevatorSystem {
private:
    vector<Elevator*> elevators;
    int min_floor;
    int max_floor;

public:
    ElevatorSystem(int min_floor, int max_floor);
    int getMinFloor();
    int getMaxFloor();
    bool isValidFloor(int floor);

    virtual void call(int floor, string command);
    void addElevator(Elevator* e);
    vector<Elevator*>& getElevators();
};

#endif