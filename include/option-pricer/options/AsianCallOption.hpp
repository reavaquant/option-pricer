#ifndef ASIANCALLOPTION_HPP
#define ASIANCALLOPTION_HPP
#include "AsianOption.hpp"

class AsianCallOption: public AsianOption {
public:
    AsianCallOption(double expiry, std::vector<double> timeSteps, double strike);
    double payoff(std::vector<double> path) const;
};

#endif