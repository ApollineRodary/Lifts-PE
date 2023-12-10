#ifndef USER_HPP
#define USER_HPP
#include <vector>
#include <optional>
#include <list>

using namespace std;

class Elevator;
class ElevatorSystem;

enum Status { IN_ELEVATOR, WAITING, IDLE };

struct Goal {
    int source_floor;
    int target_floor;
    int time;

    Goal(int source, int target, int t): source_floor(source), target_floor(target), time(t) {}
};

class User {
private:
    int floor;
    int weight;
    Status status;
    Elevator* elevator;
    ElevatorSystem& system;
    list<Goal> goals;
    optional<Goal> current_goal;
    int total_waiting_time = 0;
    int total_regret_time = 0;
    int last_request_time;
    bool is_served = false;

public:
    User(ElevatorSystem& s, int floor, int weight);
    Status getStatus();
    int getFloor();
    int getWeight();
    Goal getCurrentGoal();
    void addGoal(Goal goal);
    list<Goal> getGoals();
    void enterElevator(Elevator* elevator, int time);
    void leaveElevator(int time);
    void tick(int time);
    int getTotalWaitingTime();
    int getTotalRegretTime();
    bool getIsServed();
};

#endif