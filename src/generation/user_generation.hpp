#include "random_functions.hpp"
#include "user.hpp"
#include "elevator_system.hpp"

using namespace std;

vector<User*> user_generation(vector<float> lambdas, float mw, float sw, vector<vector<int>> targ_distrib, int tmax, ElevatorSystem& s);

void delete_generated_users(vector<User*> &users);