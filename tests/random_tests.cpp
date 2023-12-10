#include "generation/random_functions.hpp"
#include "generation/user_generation.hpp"
#include "elevator_system.hpp"
#include "user.hpp"
#include <iostream>

using namespace std;

int main() {
    // Test of a Poisson process
    cout << "POISSON PROCESS" << endl;
    vector<float> pp = poisson_process(1, 50);
    for (float arrival : pp) {
        cout << arrival << endl;
    }
    cout << endl << "Number of arrivals :" << pp.size() << endl;


    // Test of user generation
    ElevatorSystem s(0, 2);

    vector<float> lambdas = {1, 0, 0.5}; //Mean number of arrivals on a unit of time, equals to lambda
    vector<User> users = user_generation(lambdas, 1, 0, {{0, 1, 1}, {2, 0, 1}, {2, 1, 0}}, 100, s);

    vector<vector<int>> interrarival_times = {{}, {}, {}};
    vector<vector<int>> arrival_times = {{}, {}, {}};
    for (auto user : users) {
        cout << "===== USER =====" << endl
            << "Weight: " << user.getWeight() << endl
            << "Arrival time: " << user.getGoals()[0].time << endl
            << "Source floor: " << user.getGoals()[0].source_floor << endl
            << "Target floor: " << user.getGoals()[0].target_floor << endl
            << "==========" << endl;
        arrival_times[user.getGoals()[0].source_floor].push_back(user.getGoals()[0].time);
    }

    for (int i = 0 ; i < arrival_times.size() ; i++) {
        vector<int> times = arrival_times[i];
        for (int j = 1 ; j < times.size() ; j++) {
            interrarival_times[i].push_back(times[j]-times[j-1]);
        }
    }

    for (int i = 0 ; i < interrarival_times.size() ; i++) {
        vector<int> times = interrarival_times[i];
        float mean = 0;
        for (int t : times) {
            mean += t;
        }

        mean = mean/(interrarival_times[i].size());

        cout << "Excpected mean arrival time for floor " << i << ": " << 1/lambdas[i] << endl
            << "Empiric mean arrival time for floor " << i << ": " << mean << endl << endl;
    }

    return 0;
}