#include "random_functions.hpp"

random_device rd;
mt19937 gen(rd());

vector<float> poisson_process(float lambda, int tmax) {
    if (lambda == 0) {
        return {};
    }
    
    vector<float> arrivals;

    float t = 0;
    float interrarival_time;

    while (t < tmax) {
        interrarival_time = draw_exponential(lambda);
        t += interrarival_time;
        if (t < tmax) {
            arrivals.push_back(t);
        }
    }

    return arrivals;
}

int draw_discrete_distribution(vector<int>::iterator begin, vector<int>::iterator end) {
    discrete_distribution<> d(begin, end);
    return d(gen);
}

float draw_normal(float m, float s) {
    normal_distribution d{m, s};
    return d(gen);
}

double draw_exponential(float lambda) {
    exponential_distribution<double> exp (lambda);
    return exp.operator() (gen);
}