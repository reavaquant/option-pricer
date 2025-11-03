#include "CallOption.hpp"

CallOption::CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

OptionType CallOption::getOptionType() const { return OptionType::Call; }

double CallOption::payoff(double underlying) const {
    return underlying > strike_ ? underlying - strike_ : 0.0;
}