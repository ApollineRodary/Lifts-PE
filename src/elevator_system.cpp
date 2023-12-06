#include "elevator_system.hpp"

using namespace std;

ElevatorSystem::ElevatorSystem(int min_floor, int max_floor): min_floor(min_floor), max_floor(max_floor) {
    elevators = vector<Elevator*>();
}

int ElevatorSystem::getMinFloor() {
    return min_floor;
}

int ElevatorSystem::getMaxFloor() {
    return max_floor;
}

bool ElevatorSystem::isValidFloor(int floor) {
    return (floor>=min_floor && floor<=max_floor);
}

void ElevatorSystem::call(int floor, int time, string command) {};

void ElevatorSystem::addElevator(Elevator* e) {
    elevators.push_back(e);
}

vector<Elevator*>& ElevatorSystem::getElevators() {
    return elevators;
}