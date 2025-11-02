#ifndef CALLOPTION_HPP
#define CALLOPTION_HPP

#include "EuropeanVanillaOption.hpp"

class CallOption : public EuropeanVanillaOption {
public:
    CallOption(double expiry, double strike);
    OptionType getOptionType() const override;
    double payoff(double) const override;
};

#endif