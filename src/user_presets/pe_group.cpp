#include "pe_group.hpp"
#include "elevator_system.hpp"
#include "user.hpp"

PEGroup::PEGroup(ElevatorSystem& s): apo(s, 0, 1), tib(s, 4, 1), isa(s, -1, 1) {
    apo.addGoal(Goal(0, 2, 6));
    isa.addGoal(Goal(-1, 2, 23));
    tib.addGoal(Goal(4, 3, 25));
}

vector<User*> PEGroup::operator()(){
    return {&apo, &isa, &tib};
}