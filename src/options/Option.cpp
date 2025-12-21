#include "Option.h"
#include <stdexcept>
#include <vector>

Option::Option(double expiry) : expiry_(expiry) {
    if (expiry_ < 0) {
        throw std::invalid_argument("Option: expiry must be nonnegative");
    }
}

/**
 * @return the expiry time of the option.
 */
double Option::getExpiry() const {
    return expiry_;
}

/**
 * @brief Calculates the payoff path for an option.
 *
 * @param path The list of spot prices.
 *
 * @return A vector containing the payoff of the option.
 *
 * 
 * This function calculates the payoff path for an option by
 * taking the last element of the list of spot prices and
 * then calling the payoff function with this last element.
 */
std::vector<double> Option::payoffPath(const std::vector<double>& path) const {
    std::vector<double> payoff_values;
    if (!path.empty()) {
        payoff_values.push_back(payoff(path.back()));
    }
    return payoff_values;
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
