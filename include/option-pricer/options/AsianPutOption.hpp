#ifndef ASIANPUTOPTION_HPP
#define ASIANPUTOPTION_HPP
#include "AsianOption.hpp"

class AsianPutOption: public AsianOption {
public:
    AsianPutOption(double expiry, std::vector<double> timeSteps, double strike);
    double payoff(std::vector<double> path) const;
};

#endif