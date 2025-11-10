#include "EuropeanDigitalCallOption.hpp"

EuropeanDigitalCallOption::EuropeanDigitalCallOption(double expiry, double strike)
    : EuropeanDigitalOption(expiry, strike) {}

OptionType EuropeanDigitalCallOption::getOptionType() const {
    return OptionType::Call;
}

double EuropeanDigitalCallOption::payoff(double asset_price) const {
    return asset_price >= strike_ ? 1.0 : 0.0;
}
