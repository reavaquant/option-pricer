#include <algorithm>
#include "AmericanCallOption.hpp"

AmericanCallOption::AmericanCallOption(double expiry, double strike)
    : AmericanOption(expiry, strike) {}

OptionType AmericanCallOption::getOptionType() const {
    return OptionType::Call;
}

double AmericanCallOption::payoff(double spotPrice) const {
    return std::max(spotPrice - getStrike(), 0.0);
}
