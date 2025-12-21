#include "EuropeanDigitalCallOption.h"

/**
 * \brief European Digital Call Option
 *
 * This class represents a European Digital Call Option.
 *
 * It is a subclass of EuropeanDigitalOption and is used to represent a European Digital Call Option.
 *
 * The payoff of a European Digital Call Option is 1.0 if the asset price is greater than or equal to the strike price at expiry, and 0.0 otherwise.
 *
 * @param expiry The expiry date of the option.
 * @param strike The strike price of the option.
 */
EuropeanDigitalCallOption::EuropeanDigitalCallOption(double expiry, double strike) : EuropeanDigitalOption(expiry, strike) {}

/**
 * Returns the type of the option.
 *
 * This function returns OptionType::Call for a European digital call option.
 * @return the type of the option.
 */
OptionType EuropeanDigitalCallOption::getOptionType() const {
    return OptionType::Call;
}

/**
 * Returns the payoff of a European digital call option given the spot price.
 *
 * This function returns 1.0 if the asset price is greater than or equal to the strike price at expiry, and 0.0 otherwise.
 * @param asset_price the spot price of the underlying asset.
 * @return the off of the option.
 */
double EuropeanDigitalCallOption::payoff(double asset_price) const {
    return asset_price >= strike_ ? 1.0 : 0.0;
}
