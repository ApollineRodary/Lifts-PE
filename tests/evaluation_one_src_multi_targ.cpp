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
#include "algorithms/scan.cpp"
#include "generation/user_generation.hpp"

#define OUTPUT_FILENAME "../results/one_src_multiple_target.json"

int main() {
    float tick_duration = 1; //second
    float time_simu = 6000; //seconds
    float open_time = 0.8*60; //seconds
    float move_time = 0.1*60; //seconds
    //float lambda = 1./(2.*60.); //arrivals per seconds
    int max_capacity = 6;

    time_simu = round(time_simu/tick_duration);

    ScanElevatorSystem monod(0, 4);

    cout << "Total simulation time: " << time_simu << " ticks" << endl
        << "Open delay: " << ELEVATOR_OPEN_DELAY << " ticks" << endl
        << "Move delay: " << ELEVATOR_MOVE_DELAY << " ticks" << endl
        << "Tick duration: " << tick_duration << " s" << endl
        << "Max capacity: " << max_capacity << endl;
    cout << endl;

    vector<float> lambdas;
    vector<float> means;

    for (float lambda = 0.2 ; lambda <= 0.3 ; lambda += 0.2) {
        lambdas.push_back(lambda*tick_duration/60);
    }

    for (float lambda : lambdas) {
        cout << endl << "===== LAMBDA: " << lambda << " arrivals per second =====" << endl; 

        ScanElevator e(monod, 0, max_capacity);

        ELEVATOR_OPEN_DELAY = round(open_time/tick_duration); //0.8 min
        ELEVATOR_MOVE_DELAY = round(move_time/tick_duration); // 0.1 min

        
        cout << "There should be approximately " << lambda * time_simu << " arrivals" << endl;
        cout << endl;

        vector<float> lambdas = {lambda, 0, 0, 0, 0};
        vector<vector<int>> target_distrib = {
            {0, 1, 1, 1, 1},
            {1, 0, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {1, 0, 0, 0, 0},
            {1, 0, 0, 0, 0},
        };

        vector<User*> users = user_generation(lambdas, 1, 0, target_distrib, time_simu, monod);

        for (auto user : users) {
            cout << "===== USER ===== " << endl
                << "Arrival time: " << user->getGoals().front().time << endl
                << "Target floor: " << user->getGoals().front().target_floor << endl
                << "Source floor: " << user->getGoals().front().source_floor << " (should be 0)" << endl
                << "Weight: " << user->getWeight() << " (should be 1)" << endl << endl;
        }
        cout << "There are " << users.size() << " arrivals";
        cout << endl;

        Simulation sim(monod, users);
        sim.repeat(time_simu);

        float mean = 0;
        for (auto user : users) {
            cout << "User served:" << user->getIsServed() << " ; Waiting: " <<  user->getTotalWaitingTime() << endl;
            if (user->getIsServed())
                mean += user->getTotalWaitingTime();
        }
        mean /= users.size();
        mean *= tick_duration;
        mean /= 60;
        means.push_back(mean);

        delete_generated_users(users);
    }

    cout << endl << endl;
    for (int i = 0 ; i < lambdas.size() ; i++) {
        cout << "Lambda: " << lambdas[i]*60/tick_duration << endl << "Mean waiting time: " << means[i] << endl;
    }


    ofstream file(OUTPUT_FILENAME, ios_base::out);

    if (!file.is_open())
        cerr << "Unable to open file " OUTPUT_FILENAME "...\n";
    file << "{\"lambdas\": [";
    for (int i = 0 ; i < lambdas.size() ; i++) {
        file << lambdas[i]*60/tick_duration;
        if (i < lambdas.size() - 1) {
            file << ",";
        }
    }
    file << "],\"means\":[";
    for (int i = 0 ; i < means.size() ; i++) {
        file << means[i];
        if (i < means.size() - 1) {
            file << ",";
        }
    }
    file << "]}";
    file.close();
    
    return 0;
}