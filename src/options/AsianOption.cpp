#include "AsianOption.hpp"
#include <vector>

AsianOption::AsianOption(double expiry, std::vector<double> timeSteps) : Option(expiry), timeSteps_(timeSteps) {}

std::vector<double> AsianOption::getTimeSteps() {
    return timeSteps_;
}

double AsianOption::payoffPath(std::vector<double> path) const {
    double sum = 0.0;
    for (double price : path) {
        sum += price;
    }
    return sum / path.size();
}