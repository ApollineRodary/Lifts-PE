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
            switch (e->sweepDirection) {
                case NONE:
                // If the elevator has no current direction, then go to the first requested floor that is found
                for (int floor=getMinFloor(); floor<=getMaxFloor(); floor++) {
                    if (e->requestedFloors[floor-getMinFloor()]) {
                        e->setTarget(floor, time);
                        if (floor == e->getFloor())
                            e->requestedFloors[floor - getMinFloor()] = false;
                        break;
                    }
                }
                break;

                case UP:
                // If the elevator is going up, then make sure to stop at every requested floor, and only reset once there is no floor requested above
                if (e->getFloor() == getMaxFloor()) {
                    e->sweepDirection = DOWN;
                    return;
                }
                for (int floor=e->getFloor()+1; floor<=getMaxFloor(); floor++) {
                    if (e->requestedFloors[floor-getMinFloor()]) {
                        e->setTarget(floor, time);
                        break;
                    }
                    e->sweepDirection = NONE;
                }
                break;

                case DOWN:
                // If the elevator is going down, then make sure to stop at every requested floor, and only reset once there is no floor requested below
                if (e->getFloor() == getMinFloor()) {
                    e->sweepDirection = UP;
                    return;
                }
                for (int floor=e->getFloor()-1; floor>=getMinFloor(); floor--) {
                    if (e->requestedFloors[floor-getMinFloor()]) {
                        e->setTarget(floor, time);
                        break;
                    }
                    e->sweepDirection = NONE;
                }
                break;
            }
            
            if (elevator->getDirection() != NONE)
                // Sweep direction is the last direction the elevator went
                e->sweepDirection = elevator->getDirection();
            
            if (elevator->getIsOpen())
                // If the elevator is open, the current floor is no longer a target
                e->requestedFloors[elevator->getFloor() - getMinFloor()] = false;
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
