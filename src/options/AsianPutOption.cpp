#include "AsianPutOption.hpp"
#include <algorithm>
#include <utility>

AsianPutOption::AsianPutOption(double expiry, std::vector<double> timeSteps, double strike)
    : AsianOption(expiry, std::move(timeSteps)), strike_(strike) {}

double AsianPutOption::payoff(double asset_price) const {
    return std::max(strike_ - asset_price, 0.0);
}

OptionType AsianPutOption::getOptionType() const {
    return OptionType::Put;
}
