from math import *
import numpy as np
import matplotlib.pyplot as plt
import json

max_floor = 4
capacity = 6
t_floor = 0.1
t_stop = 0.3


def partition_aux(l, bins):
    if len(l)==1 or bins==1:
        yield [l]
    elif len(l)>1 and bins>1:
        for i in range(1, len(l)):
            for part in partition_aux(l[i:], bins-1):
                if len(part) + 1 == bins:
                    yield [l[:i]] + part

def partition(m, i):
    """Compute all ways of summing m positive integers to have i
    Used to compute all possible distributions of i passengers on m floors"""
    l = []
    for pa in partition_aux(range(1, i+1), m):
        li_2 = []
        for e in pa :
            li_2.append(len(e))
        l.append(li_2)
    return l

def G(m, l, i):
    """Compute the probability of making m stops on our way to floor l"""
    sum = 0
    li = partition(m, i)
    for tab in li :
        prod = 1
        sum_g = 0
        for e in range(len(tab)):
            prod *= comb(i-sum_g, tab[e])
            sum_g += tab[e]
        sum += prod
    sum *= comb(l-1, m-1)
    sum /= pow(max_floor, i)

    assert(sum <= 1)
    return sum

def expected_waiting_time(i):
    """Compute expected travel time of the elevator for i passengers"""
    sum = 0
    sum_m = 0
    for l in range(1, max_floor+1):
        for m in range(1, l+1):
            p = ((m+1)*t_stop + 2*l*t_floor)
            g = G(m, l, i)
            sum += p*g
            sum_m += m*g
    return sum
 
# Graph expected waiting time as a function of average arrival rate

x=np.arange(0, 2.2, 0.2)
mu = capacity/expected_waiting_time(capacity)
lam = x
y = 1.0/(mu-lam) + 0.6

plt.plot(x, y, label="$1/(\mu - \lambda)$")

# Graph simulation results as a function of average arrival rate

with open("results/look_one_src_multiple_target.json", "r") as json_file:
    simulation_results = json.load(json_file)["results"]
mean_look_one = sum([np.array(d["means"]) for d in simulation_results]) / len(simulation_results)
plt.plot(simulation_results[0]["lambdas"], mean_look_one, label="LOOK 1 elevator")

plt.legend()
plt.xlabel('$\lambda$ (arrivals/min)')
plt.ylabel('Expected waiting time (min)')

plt.savefig("results/model_verification")
plt.show()

plt.plot(simulation_results[0]["lambdas"], mean_look_one, label="LOOK 1 elevator")

with open("results/look_one_src_multiple_target_two_elevators.json", "r") as json_file:
    simulation_results = json.load(json_file)["results"]
mean_look_two = sum([np.array(d["means"]) for d in simulation_results]) / len(simulation_results)
plt.plot(simulation_results[0]["lambdas"], mean_look_two, label="LOOK 2 elevators")

with open("results/scan_one_src_multiple_target.json", "r") as json_file:
    simulation_results = json.load(json_file)["results"]
mean_scan_one = sum([np.array(d["means"]) for d in simulation_results]) / len(simulation_results)
plt.plot(simulation_results[0]["lambdas"], mean_scan_one, label="SCAN 1 elevator")

with open("results/scan_one_src_multiple_target_two_elevators.json", "r") as json_file:
    simulation_results = json.load(json_file)["results"]
mean_scan_two = sum([np.array(d["means"]) for d in simulation_results]) / len(simulation_results)
plt.plot(simulation_results[0]["lambdas"], mean_scan_two, label="SCAN 2 elevators")

plt.legend()
plt.xlabel('$\lambda$ (arrivals/min)')
plt.ylabel('Expected waiting time (min)')

plt.savefig("results/simulation_one_source_multiple_targets")

plt.show()
