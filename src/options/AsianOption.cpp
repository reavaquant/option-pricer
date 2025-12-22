#include "AsianOption.h"
#include <stdexcept>
#include <utility>
#include <vector>

AsianOption::AsianOption(std::vector<double> timeSteps) : Option(timeSteps.empty() ? 0.0 : timeSteps.back()), _timeSteps(std::move(timeSteps)) {
    if (_timeSteps.empty()) {
        throw std::invalid_argument("AsianOption: time steps cannot be empty");
    }
}

/**
 * @brief Returns the time steps associated with the Asian option.
 *
 * @return A vector containing the time steps associated with the Asian option.
 */
std::vector<double> AsianOption::getTimeSteps() const {
    return _timeSteps;
}

/**
 * @brief Calculates the payoff path for an Asian option.
 *
 * @param path The list of spot prices.
 *
 * @return The payoff of the option.
 *
 * 
 * This function calculates the payoff path for an Asian option by
 * taking the average of the spot prices and then calling
 * the payoff function with this average.
 */
double AsianOption::payoffPath(const std::vector<double>& path) const {
    if (path.empty()) {
        throw std::invalid_argument("AsianOption: path cannot be empty");
    }
    double sum = 0.0;
    for (double price : path) {
        sum += price;
    }
    const double avg = sum / path.size();
    return payoff(avg);
}

/**
 * @brief Returns true if the option is an Asian option.
 *
 * @return True if the option is an Asian option.
 */
bool AsianOption::isAsianOption() const {
    return true;
}
