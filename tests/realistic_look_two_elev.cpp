#include <iostream>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "user.hpp"
#include "elevator.hpp"
#include "elevator_system.hpp"
#include "simulation.hpp"
#include "log.hpp"
#include "algorithms/look.cpp"
#include "generation/user_generation.hpp"

#define OUTPUT_FILENAME "../results/one_src_multiple_target.json"

int main() {
    float tick_duration = 1; //second
    float time_simu = 6000; //seconds
    float open_time = 0.8*60; //seconds
    float move_time = 0.1*60; //seconds
    int max_capacity = 6;

    //Convert into ticks
    time_simu = round(time_simu/tick_duration);
    ELEVATOR_OPEN_DELAY = round(open_time/tick_duration);
    ELEVATOR_MOVE_DELAY = round(move_time/tick_duration);

    //Instanciate ElevatorSystem
    LookElevatorSystem monod(-1, 4);

    //Print infos
    cout << "Total simulation time: " << time_simu << " ticks" << endl
        << "Open delay: " << ELEVATOR_OPEN_DELAY << " ticks" << endl
        << "Move delay: " << ELEVATOR_MOVE_DELAY << " ticks" << endl
        << "Tick duration: " << tick_duration << " s" << endl
        << "Max capacity: " << max_capacity << endl;
    cout << endl;

    //Instanciate lambdas used for the simulation
    float mean = 0;

    //Reinstanciate an elevator
    LookElevator e1(monod, 0, max_capacity);
    LookElevator e2(monod, 0, max_capacity);
    
    //Arrivals only at the ground floor, all users go to a random floor (uniform)
    vector<float> lambdas_minutes = {1./5., 1./2., 1./10., 0., 1./7., 1./7.}; //Arrivals per minute

    for (int i = 0 ; i < lambdas_minutes.size() ; i++) {
        cout << "Floor " << i-1 << ": lambda = " << lambdas_minutes[i] << endl;
    }

    vector<float> lambdas;

    for (auto lambda_m : lambdas_minutes) {
        lambdas.push_back(lambda_m*tick_duration/60);
    }

    vector<vector<int>> target_distrib = {
        {0, 7, 1, 0, 3, 3},
        {3, 0, 3, 1, 10, 10},
        {2, 2, 0, 1, 2, 2},
        {0, 1, 0, 0, 0, 0},
        {1, 10, 1, 0, 0, 0},
        {1, 10, 1, 0, 0, 0}
    };

    vector<User*> users = user_generation(lambdas, 1, 0, target_distrib, time_simu, monod);

    //Print users infos
    for (auto user : users) {
        cout << "===== USER ===== " << endl
            << "Arrival time: " << user->getGoals().front().time << endl
            << "Target floor: " << user->getGoals().front().target_floor << endl
            << "Source floor: " << user->getGoals().front().source_floor << " (should be 0)" << endl
            << "Weight: " << user->getWeight() << " (should be 1)" << endl << endl;
    }
    cout << "There are " << users.size() << " arrivals" << endl;

    //Launch simulation
    Simulation sim(monod, users);
    sim.repeat(time_simu);

    //Compute mean waiting time
    int user_served = 0;
    for (auto user : users) {
        cout << "User served: " << user->getIsServed() << " ; Waiting time: " <<  user->getTotalWaitingTime() << endl;
        if (user->getIsServed()) {
            mean += user->getTotalWaitingTime();
            user_served++;
        }
            
    }
    mean /= user_served;
    mean *= tick_duration/60;

    //Clean memory
    delete_generated_users(users);

    //Print informations
    cout << endl << endl;
    cout << "Mean waiting time: " << mean  << " minutes" << endl;

    return 0;
}