#include "EuropeanVanillaOption.hpp"
#include <iostream>
using namespace std;

EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike) : Option(expiry), _strike(strike) {
    if (expiry < 0.0 || strike < 0.0) {
        std::cerr << "expiry and strike price must be nonnegative\n";
    }
}