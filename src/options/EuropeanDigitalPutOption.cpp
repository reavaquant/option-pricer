#include "EuropeanDigitalPutOption.hpp"

/**
 * Constructs a European digital put option with the given expiry and strike price.
 *
 * @param expiry the expiry date of the option.
 * @param strike the strike price of the option.
 */
EuropeanDigitalPutOption::EuropeanDigitalPutOption(double expiry, double strike)
    : EuropeanDigitalOption(expiry, strike) {}

/**
 * Returns the type of the option.
 *
 * This function returns OptionType::Put for a European digital put option.
 * @return the type of the option.
 */
OptionType EuropeanDigitalPutOption::getOptionType() const {
    return OptionType::Put;
}

/**
 * Returns the off of a European digital put option given the spot price.
 *
 * This function returns 1.0 if the asset price is less than or equal to the strike price at expiry, and 0.0 otherwise.
 * @param asset_price the spot price of the underlying asset.
 * @return the off of the option.
 */
double EuropeanDigitalPutOption::payoff(double asset_price) const {
    return asset_price <= strike_ ? 1.0 : 0.0;
}
