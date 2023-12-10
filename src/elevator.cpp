#include <bits/stdc++.h>
#include "elevator_system.hpp"
#include "elevator.hpp"
#include "user.hpp"
#include "log.hpp"

int ELEVATOR_OPEN_DELAY = 3;
int ELEVATOR_MOVE_DELAY = 5;

Elevator::Elevator(ElevatorSystem& system, int floor, int capacity): floor(floor), system(system), capacity(capacity), is_open(false), direction(NONE), time_since_last_update(0) {
    system.addElevator(this);
    target_floor.reset();
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
    if (is_open || (direction != NONE)) time_since_last_update++;

    if (is_open) {
        // Close the doors once they've been open for long enough
        if (time_since_last_update < ELEVATOR_OPEN_DELAY) return;
        time_since_last_update = 0;
        closeDoors(time);
        if (target_floor.has_value() && target_floor.value() != floor)
            direction = (target_floor.value()>floor)?UP:DOWN;
    } else {
        // Move one floor once the elevator has been moving for long enough
        if (direction != NONE && time_since_last_update < ELEVATOR_MOVE_DELAY) return;
        time_since_last_update = 0;
        
        if (direction==UP)
            floor++;
        else if (direction==DOWN)
            floor--;
        
        if (direction != NONE)
            debugStream("Elevator::tick") << '!' << time << '?' << "Changed floor" << '!' << floor << endl;
        
        if (target_floor.has_value() && floor == target_floor.value()) {
            direction = NONE;
            openDoors(time);
            target_floor.reset();
        }
    }
}

void Elevator::openDoors(int time) {
    assert(direction == NONE);
    debugStream("Elevator::openDoors") << '!' << time << '?' << "Opening doors" << '!' << floor << endl;
    is_open = true;
}

void Elevator::closeDoors(int time) {
    assert (direction == NONE);
    debugStream("Elevator::closeDoors") << '!' << time << '?' << "Closing doors" << '!' << floor << endl;
    is_open = false;

    // Set moving, if the elevator has a target floor
    if (!target_floor.has_value()) return;
    if (target_floor.value() > floor)
        direction = UP;
    else if (target_floor.value() < floor)
        direction = DOWN;
}

void Elevator::requestFloor(int floor, int time) {}

void Elevator::setTarget(int floor, int time) {
    assert(system.isValidFloor(floor));
    
    // If the target is the current floor, open the doors
    if (floor == this->floor) {
        assert(direction == NONE);
        openDoors(time);
        return;
    }
    
    // Otherwise, set direction of the elevator only if the elevator isn't currently open
    if (floor > this->floor) {
        assert(direction != DOWN);
        if (!is_open) direction = UP;
    } else {
        assert(direction != UP);
        if (!is_open) direction = DOWN;
    }

    target_floor = floor;
}

void Elevator::wait() {
    time_since_last_update = 0;
}

optional<int> Elevator::getTargetFloor() {
    return target_floor;
}

void Elevator::addUser(User* user) {
    assert(is_open && user->getWeight() <= getRemainingCapacity());
    users.push_back(user);
}

void Elevator::removeUser(User* user) {
    users.erase(remove(users.begin(), users.end(), user), users.end());
}