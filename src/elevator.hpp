#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <vector>
#include <cassert>
#include <optional>

using namespace std;

class User;
class ElevatorSystem;

extern int ELEVATOR_OPEN_DELAY;
extern int ELEVATOR_MOVE_DELAY;

enum Direction { UP, DOWN, NONE };

class Elevator {
private:
    vector<User*> users;
    ElevatorSystem& system;
    int floor;
    int capacity;
    int time_since_last_update;
    bool is_open;
    optional<int> target_floor;
    Direction direction;
    void openDoors(int time);
    void closeDoors(int time);

public:
    Elevator(ElevatorSystem& system, int floor, int capacity);
    
    ElevatorSystem& getElevatorSystem();
    int getFloor();
    int getCapacity();
    int getRemainingCapacity();
    bool getIsOpen();
    optional<int> getTargetFloor();
    Direction getDirection();
    vector<User*> getUsers();

    void tick(int floor);
    virtual void requestFloor(int floor, int time);
    void wait();
    void setTarget(int floor, int time);
    void addUser(User* user);
    void removeUser(User* user);
};

#endif