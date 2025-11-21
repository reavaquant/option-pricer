#include "AsianOption.hpp"
#include <vector>

AsianOption::AsianOption(double expiry, std::vector<double> timeSteps) : Option(expiry), timeSteps_(timeSteps) {}

std::vector<double> AsianOption::getTimeSteps() const {
    return timeSteps_;
}

std::vector<double> AsianOption::payoffPath(const std::vector<double>& path) const {
    if (path.empty()) return {};
    double sum = 0.0;
    for (double price : path) {
        sum += price;
    }
    const double avg = sum / path.size();
    return {payoff(avg)};
}

bool AsianOption::isAsianOption() const {
    return true;
}
