#include "CallOption.hpp"
#include "EuropeanVanillaOption.hpp"
using namespace std;

CallOption::CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

OptionType CallOption::getOptionType() const {
    return OptionType::Call;
}

double CallOption::payoff(double underlying) const {
    if (underlying > _strike) {
        return underlying - _strike;
    } else {
        return 0.0;
    }
}