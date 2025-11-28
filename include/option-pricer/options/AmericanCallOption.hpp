#ifndef AMERICANCALLOPTION_HPP
#define AMERICANCALLOPTION_HPP

#include <algorithm>
#include "AmericanOption.hpp"

class AmericanCallOption : public AmericanOption {
public:
    AmericanCallOption(double expiry, double strike) : AmericanOption(expiry, strike) {}

    OptionType getOptionType() const override {
        return OptionType::Call;
    }

    double payoff(double spotPrice) const override {
        return std::max(spotPrice - getStrike(), 0.0);
    }
};

#endif
