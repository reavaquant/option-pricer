#ifndef CALLOPTION_H
#define CALLOPTION_H

#include "EuropeanVanillaOption.h"

class CallOption : public EuropeanVanillaOption {
public:
    CallOption(double expiry, double strike);
    OptionType getOptionType() const override;
    double payoff(double asset_price) const override;
};

#endif