#include "PutOption.hpp"

PutOption::PutOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

OptionType PutOption::getOptionType() const { return OptionType::Put; }

double PutOption::payoff(double asset_price) const {
    return strike_ >= asset_price ? strike_ - asset_price : 0.0;
}