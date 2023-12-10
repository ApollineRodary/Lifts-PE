from math import *
import numpy as np
import matplotlib.pyplot as plt

N= 4 #number of floors minus 1 (=number of floors that can be requested)
c= 6 #number of passengers in the elevator
t_floor=0.1 #time to move from one floor to another
t_stop=0.3 #time pass on one floor to drop off passengers


# <<<<<< Partition function >>>>>>
# The function allows you to obtain 
# all possible ways of summing m positive integers to have i.

# This allows you to obtain all possible distribution
# of the passengers on the k stops.
def partition(lista,bins):
    if len(lista)==1 or bins==1:
        yield [lista]
    elif len(lista)>1 and bins>1:
        for i in range(1,len(lista)):
            for part in partition(lista[i:],bins-1):
                if len([lista[:i]]+part)==bins:
                    yield [lista[:i]]+part

def part2(m,i):
    li=[]
    for pa in partition(range(1,i+1),m):
        li_2= []
        for e in pa :
            li_2.append(len(e))
        li.append(li_2)
    return li


# <<<<<< G function >>>>>>
# This function allows us to calculate the probability 
# of going to floor l by making m stops.
def G(m,l,i):
    sum =0
    li=part2(m,i)
    for tab in li :
        prod=1
        sum_g=0
        for e in range(len(tab)):
            prod=prod*comb(i-sum_g, tab[e])
            sum_g=sum_g+tab[e]
        sum=sum +prod
    sum=sum*comb(l-1, m-1)
    sum=sum/ pow(N, i)
    print("proba result =",sum)
    if sum >1 :
        print("ERROR PROBA")
    return sum

# Expectation function
#This function allows us to calculate the expectation 
#of travel time of the elevator for i passangers
def exp(i):
    sum=0
    for l in range(1,N+1):
        for m in range(1,l+1):
            print("m=",m,"l=",l)
            p=((m+1)*t_stop + 2*l*t_floor)
            print("travel time =",p)
            q=p*G(m,l,i)
            sum=sum + q
    return sum


#Graph Function
#This function create the graph of the expectation as a function of i 
mu = 6.0/exp(c)
x=np.arange(0, 2.2, 0.2)
lam=x
y=1.0/(mu-lam)
plt.plot(x, y)
mu_prime = 1.0/exp(c)
x_prime=np.arange(0, 2.2, 0.2)
lam_prime=x/6
y_prime=1.0/(mu_prime-lam_prime)
plt.plot(x,y_prime)
#plt.title('The graph of the expectation as a function of i')
plt.xlabel('Value of $\lambda$ (passengers/minutes)')
plt.ylabel('The expectation of waiting time (in minutes)')
plt.show()

l=0.2
print("exp time model_2", 1.0/(mu-l))
print("exp time model_1", 1.0/((mu/6)-l/6))
#print("Expectation of travel time of the elevator for i passangers",exp(6))
