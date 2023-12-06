#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <vector>
#include <cassert>

using namespace std;

class User;
class ElevatorSystem;

extern int ELEVATOR_OPEN_DELAY;
extern int ELEVATOR_MOVE_DELAY;

class Elevator {
private:
    vector<User*> users;
    ElevatorSystem& system;
    int floor;
    int capacity;
    int target_floor;
    int time_since_last_update;
    bool is_open;
    bool is_moving;

public:
    Elevator(ElevatorSystem& system, int floor);
    
    int getFloor();
    int getCapacity();
    int getRemainingCapacity();
    bool getIsOpen();
    vector<User*> getUsers();

    virtual void tick(int floor);
    virtual void requestFloor(int floor);
    void moveToFloor(int floor);
    void addUser(User* user);
    void removeUser(User* user);
};

#endif