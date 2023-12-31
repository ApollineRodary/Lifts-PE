#include "user.hpp"
#include "elevator.hpp"
#include "elevator_system.hpp"
#include "simulation.hpp"
#include "log.hpp"

using namespace std;

class LookElevator: public Elevator {
public:
    vector<bool> requestedFloors;
    Direction sweepDirection;

    LookElevator(ElevatorSystem& system, int floor, int capacity): Elevator(system, floor, capacity) {
        sweepDirection = NONE;
        requestedFloors = vector<bool>(system.getMaxFloor()-system.getMinFloor()+1, false);
    }

    void requestFloor(int floor, int time) override {
        requestedFloors[floor - getElevatorSystem().getMinFloor()] = true;
        debugStream("LookElevator::requestFloor") << '!' << time << '?' << "Asked for floor" << '!' << floor << endl;
    }
};

class LookElevatorSystem: public ElevatorSystem {
public:
    LookElevatorSystem(int min_floor, int max_floor): ElevatorSystem(min_floor, max_floor) {};

    void tick(int time) override {
        for (auto elevator: getElevators()) {
            LookElevator* e = (LookElevator*) elevator;
            
            // Forget requests for the current floor once the doors are open
            if (e->getIsOpen())
                e->requestedFloors[e->getFloor() - getMinFloor()] = false;

            switch (e->sweepDirection) {
                bool requests_left;
                
                case NONE:
                    if (elevator->getDirection() != NONE) {
                        // Sync sweep direction and direction, if the elevator is already moving
                        e->sweepDirection = elevator->getDirection();
                        continue;
                    }
                    for (int floor=getMinFloor(); floor<=getMaxFloor(); floor++) {
                        // Otherwise, get moving to the first requested floor that is found
                        if (e->requestedFloors[floor-getMinFloor()]) {
                            e->setTarget(floor, time);
                            e->sweepDirection = elevator->getDirection();
                            break;
                        }
                    }
                break;

                case UP:
                    // If the elevator is going up, then go to the next requested floor above, and reset once there is no request left
                    requests_left = false;
                    for (int floor=e->getFloor()+1; floor<=getMaxFloor(); floor++) {
                        if (e->requestedFloors[floor-getMinFloor()]) {
                            e->setTarget(floor, time);
                            requests_left = true;
                            break;
                        }
                    }
                    if (!requests_left)
                        e->sweepDirection = NONE;
                break;
                
                case DOWN:
                    // If the elevator is going down, then make sure to stop at every requested floor, and only reset once there is no floor requested below
                    requests_left = false;
                    for (int floor=e->getFloor()-1; floor>=getMinFloor(); floor--) {
                        if (e->requestedFloors[floor-getMinFloor()]) {
                            e->setTarget(floor, time);
                            requests_left = true;
                            break;
                        }
                    }
                    if (!requests_left)
                        e->sweepDirection = NONE;
                break;
            }
        }
    }

    void call(int floor, int time, string command) override {
        for (auto elevator: getElevators())
            if (elevator->getFloor() != floor || !elevator->getTargetFloor().has_value())
                elevator->requestFloor(floor, time);
            else
                elevator->wait();
    }
};
