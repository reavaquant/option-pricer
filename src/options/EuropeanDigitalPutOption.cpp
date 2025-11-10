#include "EuropeanDigitalPutOption.hpp"

EuropeanDigitalPutOption::EuropeanDigitalPutOption(double expiry, double strike)
    : EuropeanDigitalOption(expiry, strike) {}

OptionType EuropeanDigitalPutOption::getOptionType() const {
    return OptionType::Put;
}

double EuropeanDigitalPutOption::payoff(double asset_price) const {
    return asset_price <= strike_ ? 1.0 : 0.0;
}
