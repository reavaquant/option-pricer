#include "Option.hpp"
#include <vector>

Option::Option(double expiry) : expiry_(expiry) {}

double Option::getExpiry() const {
    return expiry_;
}

std::vector<double> Option::payoffPath(const std::vector<double>& path) const {
    std::vector<double> payoff_values;
    if (!path.empty()) {
        payoff_values.push_back(payoff(path.back()));
    }
    return payoff_values;
}