#include "CallOption.hpp"

CallOption::CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

OptionType CallOption::getOptionType() const { return OptionType::Call; }

double CallOption::payoff(double asset_price) const {
    return asset_price > strike_ ? asset_price - strike_ : 0.0;
}