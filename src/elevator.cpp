#include <bits/stdc++.h>
#include "elevator_system.hpp"
#include "elevator.hpp"
#include "user.hpp"
#include "log.hpp"

int ELEVATOR_OPEN_DELAY = 3;
int ELEVATOR_MOVE_DELAY = 5;

Elevator::Elevator(ElevatorSystem& system, int floor): floor(floor), system(system) {
    system.addElevator(this);
    is_open = false;
    is_moving = false;
    time_since_last_update = 0;
    target_floor = floor;
};

int Elevator::getFloor() {
    return floor;
}

int Elevator::getCapacity() {
    return capacity;
}

int Elevator::getRemainingCapacity() {
    int total_weight = 0;
    for (User* u: getUsers())
        total_weight += u->getWeight();
    return getCapacity() - total_weight;
}

bool Elevator::getIsOpen() {
    return is_open;
}

vector<User*> Elevator::getUsers() {
    return users;
}

void Elevator::tick(int time) {
    if (is_open || is_moving || target_floor != floor) time_since_last_update++;

    if (is_open) {
        // Close the doors once they've been open for long enough
        if (time_since_last_update < ELEVATOR_OPEN_DELAY) return;
        time_since_last_update = 0;
        debugStream("Elevator::tick") << '!' << time << '?' << "Closing doors" << '!' << floor << endl;
        is_open = false;
        if (target_floor != floor)
            is_moving = true;
    } else {
        // Move one floor once the elevator has been moving for long enough
        if (time_since_last_update < ELEVATOR_MOVE_DELAY) return;
        time_since_last_update = 0;
        
        if (floor < target_floor)
            floor++;
        else if (floor > target_floor)
            floor--;

        debugStream("Elevator::tick") << '!' << time << '?' << "Changed floor" << '!' << floor << endl;
        
        if (floor == target_floor) {
            debugStream("Elevator::tick") << '!' << time << '?' << "Opening doors" << '!' << floor << endl;
            is_open = true;
            is_moving = false;
        }
    }
}

void Elevator::requestFloor(int floor) {}

void Elevator::moveToFloor(int floor) {
    assert(system.isValidFloor(floor));
    target_floor = floor;
}

void Elevator::addUser(User* user) {
    assert(user->getWeight() <= getRemainingCapacity());
    users.push_back(user);
}

void Elevator::removeUser(User* user) {
    remove(users.begin(), users.end(), user);
}