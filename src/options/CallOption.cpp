#include "CallOption.hpp"

CallOption::CallOption(double expiry, double strike) : EuropeanVanillaOption(expiry, strike) {}

OptionType CallOption::getOptionType() const { return OptionType::Call; }

/**
 * Returns the payoff of a call option given the spot price.
 *
 * This function returns the maximum of the difference between the spot price
 * and the strike price, or 0.0 if the spot price is less than or equal to the strike price.
 * @param asset_price the spot price of the underlying asset.
 * @return the off of the option.
 */
double CallOption::payoff(double asset_price) const {
    return asset_price > strike_ ? asset_price - strike_ : 0.0;
}