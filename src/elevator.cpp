#include <bits/stdc++.h>
#include "elevator_system.hpp"
#include "elevator.hpp"
#include "user.hpp"
#include "log.hpp"

int ELEVATOR_OPEN_DELAY = 3;
int ELEVATOR_MOVE_DELAY = 5;

Elevator::Elevator(ElevatorSystem& system, int floor, int capacity): floor(floor), system(system), capacity(capacity) {
    system.addElevator(this);
    is_open = false;
    direction = NONE;
    time_since_last_update = 0;
    target_floor = floor;
};

ElevatorSystem& Elevator::getElevatorSystem() {
    return system;
}

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

Direction Elevator::getDirection() {
    return direction;
}

vector<User*> Elevator::getUsers() {
    return users;
}

void Elevator::tick(int time) {
    if (is_open || (direction != NONE) || target_floor != floor) time_since_last_update++;

    if (is_open) {
        // Close the doors once they've been open for long enough
        if (time_since_last_update < ELEVATOR_OPEN_DELAY) return;
        time_since_last_update = 0;
        debugStream("Elevator::tick") << '!' << time << '?' << "Closing doors" << '!' << floor << endl;
        is_open = false;
        if (target_floor != floor)
            direction = (target_floor>floor)?UP:DOWN;
    } else {
        // Move one floor once the elevator has been moving for long enough
        if (time_since_last_update < ELEVATOR_MOVE_DELAY) return;
        time_since_last_update = 0;
        
        if (direction==UP)
            floor++;
        else if (direction==DOWN)
            floor--;

        debugStream("Elevator::tick") << '!' << time << '?' << "Changed floor" << '!' << floor << endl;
        
        if (floor == target_floor) {
            debugStream("Elevator::tick") << '!' << time << '?' << "Opening doors" << '!' << floor << endl;
            is_open = true;
            direction = NONE;
        }
    }
}

void Elevator::requestFloor(int floor, int time) {}

void Elevator::setTarget(int floor, int time) {
    assert(system.isValidFloor(floor));
    
    if (target_floor != floor)
        debugStream("Elevator::moveToFloor") << '!' << time << '?' << "New target" << '!' << floor << endl;
    
    if (floor > this->floor) {
        assert (direction != DOWN);
        direction = UP;
    } else if (floor < this->floor) {
        assert (direction != UP);
        direction = DOWN;
    }

    target_floor = floor;
}

void Elevator::wait() {
    time_since_last_update = 0;
}

void Elevator::addUser(User* user) {
    assert(user->getWeight() <= getRemainingCapacity());
    users.push_back(user);
}

void Elevator::removeUser(User* user) {
    remove(users.begin(), users.end(), user);
}