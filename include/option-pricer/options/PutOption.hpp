#ifndef PUTOPTION_HPP
#define PUTOPTION_HPP

#include "EuropeanVanillaOption.hpp"

class PutOption : public EuropeanVanillaOption {
public:
    PutOption(double expiry, double strike);
    OptionType getOptionType() const override;
    double payoff(double) const override;
};

#endif