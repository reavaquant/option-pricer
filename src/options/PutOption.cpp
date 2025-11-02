#include "PutOption.hpp"

PutOption::PutOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

OptionType PutOption::getOptionType() const { return OptionType::Put; }

double PutOption::payoff(double underlying) const {
    return _strike >= underlying ? _strike - underlying : 0.0;
}