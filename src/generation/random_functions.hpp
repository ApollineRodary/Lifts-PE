#include <vector>
#include <string>
#include <random>

using namespace std;

//int random_dist(vector<int> distribution); : No use : use std::discrete_distribution insetad;
vector<float> poisson_process(float lambda, int tmax);
int draw_discrete_distribution(vector<int>::iterator begin, vector<int>::iterator end);
float draw_normal(float m, float s);
double draw_exponential(float lambda);

extern random_device rd;