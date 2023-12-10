#include <vector>
#include "user.hpp"

using namespace std;

class ElevatorSystem;

class PEGroup {
    User apo, tib, isa;

public:
    PEGroup(ElevatorSystem& s);
    vector<User*> operator()();
};