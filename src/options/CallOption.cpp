#include "CallOption.hpp"
#include "EuropeanVanillaOption.hpp"
using namespace std;

CallOption::CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

OptionType CallOption::getOptionType() const { return OptionType::Call; }

double CallOption::payoff(double underlying) const {
    return underlying > _strike ? underlying - _strike : 0.0;
}