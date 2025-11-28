#include "AsianPutOption.h"
#include <algorithm>
#include <utility>

/**
 * Constructor for AsianPutOption.
 *
 * @param expiry the time to expiry of the option.
 * @param timeSteps the time steps of the option.
 * @param strike the strike price of the option.
 */
AsianPutOption::AsianPutOption(std::vector<double> timeSteps, double strike)
    : AsianOption(std::move(timeSteps)), strike_(strike) {}

/**
 * Returns the payoff of an Asian put option given the spot price.
 *
 * This function returns the maximum of the difference between the strike price
 * and the spot price, or 0.0 if the spot price is greater than or equal to the strike price.
 * @param asset_price the spot price of the underlying asset.
 * @return the payoff of the option.
 */
double AsianPutOption::payoff(double asset_price) const {
    return std::max(strike_ - asset_price, 0.0);
}

/**
 * Returns the type of the option.
 *
 * This function returns OptionType::Put for an Asian put option.
 *
 * @return the type of the option.
 */
OptionType AsianPutOption::getOptionType() const {
    return OptionType::Put;
}
