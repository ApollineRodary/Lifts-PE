#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include <vector>
#include <cassert>

using namespace std;

class User;
class ElevatorSystem;

class Elevator {
private:
    vector<User*> users;
    ElevatorSystem& system;
    int floor;
    int capacity;
    bool is_open;

public:
    Elevator();
    
    int getFloor();
    int getCapacity();
    int getRemainingCapacity();
    bool getIsOpen();
    vector<User*> getUsers();

    virtual void requestFloor(int floor);
    void moveToFloor(int floor);
    void addUser(User* user);
    void removeUser(User* user);
};

#endif