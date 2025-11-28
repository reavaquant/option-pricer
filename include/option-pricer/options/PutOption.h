#ifndef PUTOPTION_H
#define PUTOPTION_H

#include "EuropeanVanillaOption.h"

class PutOption : public EuropeanVanillaOption {
public:
    PutOption(double expiry, double strike);
    OptionType getOptionType() const override;
    double payoff(double asset_price) const override;
};

#endif