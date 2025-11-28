#include <algorithm>
#include "AmericanPutOption.hpp"

AmericanPutOption::AmericanPutOption(double expiry, double strike)
    : AmericanOption(expiry, strike) {}

OptionType AmericanPutOption::getOptionType() const {
    return OptionType::Put;
}

double AmericanPutOption::payoff(double spotPrice) const {
    return std::max(getStrike() - spotPrice, 0.0);
}
