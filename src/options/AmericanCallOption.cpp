#include <algorithm>
#include "AmericanCallOption.h"

/**
 * Constructor for an American call option.
 *
 * @param expiry the expiry of the option.
 * @param strike the strike price of the option.
 */
AmericanCallOption::AmericanCallOption(double expiry, double strike) : AmericanOption(expiry, strike) {}

/**
 * Returns the type of the option.
 *
 * This function returns OptionType::Call for an American call option.
 *
 * @return the type of the option.
 */
OptionType AmericanCallOption::getOptionType() const {
    return OptionType::Call;
}

/**
 * Returns the payoff of an American call option given the spot price.
 *
 * This function simply returns the maximum of the difference between the spot price
 * and the strike price, or 0.0 if the spot price is less than or equal to the strike price.
 *
 * @param spotPrice the spot price of the underlying asset.
 * @return the payoff of the option.
 */
double AmericanCallOption::payoff(double spotPrice) const {
    return std::max(spotPrice - getStrike(), 0.0);
}
