#include <iostream>
#include "CallOption.hpp"
#include "PutOption.hpp"

int main() {
    CallOption call(1.0, 100.0);
    PutOption put(1.0, 100.0);

    std::cout << "Call payoff @110 = " << call.payoff(110.0) << "\n";
    std::cout << "Put payoff @90  = " << put.payoff(90.0) << "\n";
    return 0;
}