#include <cassert>
#include <iostream>

#include "user.hpp"
#include "elevator.hpp"
#include "elevator_system.hpp"
#include "log.hpp"

using namespace std;

User::User(ElevatorSystem& s, int floor, int weight) : system(s), floor(floor), weight(weight) {
    goals = vector<Goal>();
    status = IDLE;
    current_goal.reset();
}

Status User::getStatus() {
    return status;
}

int User::getFloor() {
    return floor;
}

int User::getWeight() {
    return weight;
}

Goal User::getCurrentGoal() {
    assert(current_goal.has_value());
    return current_goal.value();
}

void User::addGoal(Goal goal) {
    goals.push_back(goal);
}

void User::enterElevator(Elevator* e) {
    assert(getFloor() == e->getFloor());
    e->addUser(this);
    elevator = e;
    status = IN_ELEVATOR;
}

void User::leaveElevator() {
    floor = elevator->getFloor();
    elevator->removeUser(this);
    elevator = nullptr;
    current_goal.reset();
    status = IDLE;
}

void User::tick(int time) {
    switch(status) {
        case IDLE:
        // See if there are goals for the current tick
        for (Goal goal: goals) {
            if (goal.time != time)
                continue;
            current_goal = goal;
            assert(goal.source_floor == floor);
            if (goal.target_floor > floor)
                system.call(floor, time, "CALL_ELEVATOR_UP");
            else
                system.call(floor, time, "CALL_ELEVATOR_DOWN");
            status = WAITING;
            return;
        }
        break;

        case WAITING:
        // See if there is an elevator open at the current floor with enough room
        for (Elevator* e: system.getElevators()) {
            if (!e->getIsOpen() || (e->getFloor() != floor) || (getWeight() > e->getRemainingCapacity()))
                continue;
            enterElevator(e);
            debugStream("User::tick") << '!' << time << '?' << "Entering elevator" << '!' << floor << endl;
            e->requestFloor(current_goal.value().target_floor, time);
            return;
        }
        break;

        case IN_ELEVATOR:
        // See if the current elevator is at the target floor
        if (elevator->getFloor() == current_goal.value().target_floor)
            leaveElevator();
        break;
    }
}