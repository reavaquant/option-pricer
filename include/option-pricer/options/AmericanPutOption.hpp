#ifndef AMERICANPUTOPTION_HPP
#define AMERICANPUTOPTION_HPP

#include <algorithm>
#include "AmericanOption.hpp"

class AmericanPutOption : public AmericanOption {
public:
    AmericanPutOption(double expiry, double strike) : AmericanOption(expiry, strike) {}

    OptionType getOptionType() const override {
        return OptionType::Put;
    }

    double payoff(double spotPrice) const override {
        return std::max(getStrike() - spotPrice, 0.0);
    }
};

#endif
