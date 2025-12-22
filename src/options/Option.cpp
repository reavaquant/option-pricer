#include "Option.h"
#include <stdexcept>
#include <vector>

Option::Option(double expiry) : _expiry(expiry) {
    if (_expiry < 0) {
        throw std::invalid_argument("Option: expiry must be nonnegative");
    }
}

/**
 * @return the expiry time of the option.
 */
double Option::getExpiry() const {
    return _expiry;
}

std::vector<double> Option::getTimeSteps() const {
    return {_expiry};
}

/**
 * @brief Calculates the payoff path for an option.
 *
 * @param path The list of spot prices.
 *
 * @return The payoff of the option.
 */
double Option::payoffPath(const std::vector<double>& path) const {
    if (path.empty()) {
        throw std::invalid_argument("Option: path cannot be empty");
    }
    return payoff(path.back());
}

/**
 * @brief Returns true if the option is an Asian option.
 *
 * @return False if the option is not an Asian option.
 */
bool Option::isAsianOption() const {
    return false;
}

/**
 * @brief Returns true if the option is an American option.
 *
 * @return False if the option is not an American option.
 */
bool Option::isAmericanOption() const {
    return false;
}
