#include <algorithm>
#include "AmericanPutOption.h"

/**
 * Constructor for an American put option.
 *
 * @param expiry the expiry of the option.
 * @param strike the strike price of the option.
 */
AmericanPutOption::AmericanPutOption(double expiry, double strike) : AmericanOption(expiry, strike) {}

/**
 * Returns the type of the option.
 *
 * This function returns OptionType::Put for an American put option.
 * @return the type of the option.
 */
OptionType AmericanPutOption::getOptionType() const {
    return OptionType::Put;
}

/**
 * Returns the payoff of an American put option given the spot price.
 *
 * This function returns the maximum of the difference between the strike price
 * and the spot price, or 0.0 if the spot price is greater than or equal to the strike price.
 * @param spotPrice the spot price of the underlying asset.
 * @return the payoff of the option.
 */
double AmericanPutOption::payoff(double spotPrice) const {
    return std::max(getStrike() - spotPrice, 0.0);
}
