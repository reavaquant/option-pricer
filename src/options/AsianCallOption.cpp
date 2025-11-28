#include "AsianCallOption.h"
#include <algorithm>
#include <utility>

AsianCallOption::AsianCallOption(std::vector<double> timeSteps, double strike) : AsianOption(std::move(timeSteps)), strike_(strike) {}

/**
 * @brief The payoff of an Asian call option.
 *
 * @param asset_price The price of the underlying asset.
 * @return The payoff of the option.
 */
double AsianCallOption::payoff(double asset_price) const {
    return std::max(asset_price - strike_, 0.0);
}

/**
 * Returns the type of the option.
 *
 * This function returns OptionType::Call for an Asian call option.
 * @return the type of the option.
 */
OptionType AsianCallOption::getOptionType() const {
    return OptionType::Call;
}
