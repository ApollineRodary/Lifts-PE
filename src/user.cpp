#include <cassert>
#include <iostream>

#include "user.hpp"
#include "elevator.hpp"
#include "elevator_system.hpp"
#include "log.hpp"

using namespace std;

User::User(ElevatorSystem& s, int floor, int weight) : system(s), floor(floor), weight(weight) {
    goals = list<Goal>();
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

list<Goal> User::getGoals() {
    return goals;
}

int User::getTotalWaitingTime() {
    return total_waiting_time;
}

int User::getTotalRegretTime() {
    return total_regret_time;
}

int User::getTotalWaitingTimeSquared() {
    return total_waiting_time_squared;
}

bool User::getIsServed() {
    return is_served;
}

void User::enterElevator(Elevator* e, int time) {
    assert(getFloor() == e->getFloor());
    debugStream("User::enterElevator") << '!' << time << '?' << "Entering elevator" << '!' << floor << endl;
    e->addUser(this);
    elevator = e;
    status = IN_ELEVATOR;
}

void User::leaveElevator(int time) {
    total_waiting_time += time - last_request_time - 1;
    total_waiting_time_squared += (time-last_request_time-1)*(time-last_request_time-1);
    total_regret_time += time - last_request_time -  ELEVATOR_MOVE_DELAY * abs((current_goal.value().target_floor - current_goal.value().source_floor)) - ELEVATOR_OPEN_DELAY - 1;
    is_served = true;

    floor = elevator->getFloor();
    debugStream("User::leaveElevator") << '!' << time << '?' << "Leaving elevator" << '!' << floor << endl;

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
            if (goal.time > time)
                continue;
            current_goal = goal;
            goals.pop_front();
            assert(goal.source_floor == floor);
            if (goal.target_floor > floor)
                system.call(floor, time, "CALL_ELEVATOR_UP");
            else
                system.call(floor, time, "CALL_ELEVATOR_DOWN");

            last_request_time = time;
            status = WAITING;
            return;
        }
        break;

        case WAITING:
        // See if there is an elevator open at the current floor with enough room
        for (Elevator* e: system.getElevators()) {
            if (getWeight() > e->getRemainingCapacity())
                debugStream("User::tick") << '!' << time << '?' << "Full elevator" << endl;
            if (!e->getIsOpen() || (e->getFloor() != floor) || (getWeight() > e->getRemainingCapacity()))
                continue;
            enterElevator(e, time);
            e->requestFloor(current_goal.value().target_floor, time);
            return;
        }
        break;

        case IN_ELEVATOR:
        // See if the current elevator is at the target floor
        if (elevator->getFloor() == current_goal.value().target_floor)
            leaveElevator(time);
        break;
    }
}