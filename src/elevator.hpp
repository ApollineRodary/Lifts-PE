#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <vector>
#include <cassert>

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
    int target_floor;
    int time_since_last_update;
    bool is_open;
    Direction direction;

public:
    Elevator(ElevatorSystem& system, int floor, int capacity);
    
    ElevatorSystem& getElevatorSystem();
    int getFloor();
    int getCapacity();
    int getRemainingCapacity();
    bool getIsOpen();
    Direction getDirection();
    vector<User*> getUsers();

    void tick(int floor);
    virtual void requestFloor(int floor, int time);
    void setTarget(int floor, int time);
    void addUser(User* user);
    void removeUser(User* user);
};

#endif