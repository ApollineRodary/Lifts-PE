#include <bits/stdc++.h>
#include "elevator_system.hpp"
#include "elevator.hpp"
#include "user.hpp"

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

void Elevator::requestFloor(int floor) {}

void Elevator::moveToFloor(int floor) {
    assert(system.isValidFloor(floor));
    this->floor = floor;
}

void Elevator::addUser(User* user) {
    assert(user->getWeight() <= getRemainingCapacity());
    users.push_back(user);
}

void Elevator::removeUser(User* user) {
    remove(users.begin(), users.end(), user);
}