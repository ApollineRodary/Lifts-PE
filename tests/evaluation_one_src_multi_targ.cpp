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
    int nb_tests = 50;
    
    float tick_duration = 1; //second
    float time_simu = 12000; //seconds
    float open_time = 0.3*60; //seconds
    float move_time = 0.1*60; //seconds
    int max_capacity = 6;

    //Convert into ticks
    time_simu = round(time_simu/tick_duration);
    ELEVATOR_OPEN_DELAY = round(open_time/tick_duration);
    ELEVATOR_MOVE_DELAY = round(move_time/tick_duration);

    //Print infos
    cout << "Total simulation time: " << time_simu << " ticks" << endl
        << "Open delay: " << ELEVATOR_OPEN_DELAY << " ticks" << endl
        << "Move delay: " << ELEVATOR_MOVE_DELAY << " ticks" << endl
        << "Tick duration: " << tick_duration << " s" << endl
        << "Max capacity: " << max_capacity << endl;
    cout << endl;

    //Instanciate lambdas used for the simulation
    vector<float> lambdas;

    for (float lambda = 0.2 ; lambda <= 2.01 ; lambda += 0.2)
        lambdas.push_back(lambda*tick_duration/60);
    
    ofstream file(OUTPUT_FILENAME, ios_base::out);

    if (!file.is_open())
        cerr << "Unable to open file " OUTPUT_FILENAME "...\n";
    
    file << "{\"results\":[" << endl;

    for (int _ = 0 ; _ < nb_tests ; _++) {
        //Launch the simulations
        vector<float> means;
        for (float lambda : lambdas) {
            cout << endl << "===== LAMBDA: " << lambda << " arrivals per second =====" << endl; 

            //Reinstanciate ElevatorSystem
            LookElevatorSystem monod(0, 4);

            //Reinstanciate an elevator
            LookElevator e(monod, 0, max_capacity);
            
            //Informations
            cout << "There should be approximately " << lambda * time_simu << " arrivals" << endl << endl;

            //Arrivals only at the ground floor, all users go to a random floor (uniform)
            vector<float> lambdas = {lambda, 0, 0, 0, 0};
            vector<vector<int>> target_distrib = {
                {0, 1, 1, 1, 1},
                {1, 0, 0, 0, 0},
                {1, 0, 0, 0, 0},
                {1, 0, 0, 0, 0},
                {1, 0, 0, 0, 0},
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

            //Clean memory
            delete_generated_users(users);
        }

        //Print informations
        cout << endl << endl;
        for (int i = 0 ; i < lambdas.size() ; i++) {
            cout << "Lambda: " << lambdas[i]*60/tick_duration << endl << "Mean waiting time: " << means[i] << endl;
        }

        //Write json file
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
        if (_ < nb_tests - 1)
            file << "," << endl;
    }
    file << endl << "]}";
    file.close();
    
    
    return 0;
}