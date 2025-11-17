#include "AsianCallOption.hpp"
#include <algorithm>
#include <utility>

AsianCallOption::AsianCallOption(double expiry, std::vector<double> timeSteps, double strike) : AsianOption(expiry, std::move(timeSteps)), strike_(strike) {}

double AsianCallOption::payoff(double asset_price) const {
    return std::max(asset_price - strike_, 0.0);
}

OptionType AsianCallOption::getOptionType() const {
    return OptionType::Call;
}
