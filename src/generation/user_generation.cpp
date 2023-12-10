#include "user_generation.hpp"


vector<User> user_generation(vector<float> lambdas, float mw, float sw, vector<vector<int>> targ_distrib, int tmax, ElevatorSystem& s) {
    //float t = 0;
    vector<User> users;

    for (int src_floor = 0 ; src_floor < lambdas.size() ; src_floor++) {
        vector<float> times = poisson_process(lambdas[src_floor], tmax);
        
        for (int u = 0 ; u < times.size() ; u++) {
            //Draw a weight with a gaussian distribution
            int weight = round(draw_normal(mw, sw));

            int target = draw_discrete_distribution(targ_distrib[src_floor].begin(), targ_distrib[src_floor].end());

            User user(s, src_floor-s.getMinFloor(), weight);
            Goal goal(src_floor-s.getMinFloor(), target-s.getMinFloor(), floor(times[u]));
            user.addGoal(goal);

            users.push_back(user);
        }
    }

    return users;
}