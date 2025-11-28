#include "PutOption.hpp"

PutOption::PutOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

OptionType PutOption::getOptionType() const { return OptionType::Put; }

/**
 * @brief The payoff of a put option.
 *
 * @param asset_price The price of the underlying asset.
 * @return The off of the option.
 */
double PutOption::payoff(double asset_price) const {
    return strike_ >= asset_price ? strike_ - asset_price : 0.0;
}