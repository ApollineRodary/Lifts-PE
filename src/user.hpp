#ifndef USER_HPP
#define USER_HPP
#include <vector>
#include <optional>

using namespace std;

class Elevator;
class ElevatorSystem;

enum Status { IN_ELEVATOR, WAITING, IDLE };

struct Goal {
    int source_floor;
    int target_floor;
    int time;
};

class User {
private:
    int floor;
    int weight;
    Status status;
    Elevator* elevator;
    ElevatorSystem& system;
    vector<Goal> goals;
    optional<Goal> current_goal;

public:
    User(ElevatorSystem& s);
    Status getStatus();
    int getFloor();
    int getWeight();
    Goal getCurrentGoal();
    void addGoal(Goal goal);
    void enterElevator(Elevator* elevator);
    void leaveElevator();
    void tick(int time);
};

#endif