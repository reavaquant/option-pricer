#ifndef AMERICANCALLOPTION_H
#define AMERICANCALLOPTION_H

#include "AmericanOption.h"

class AmericanCallOption : public AmericanOption {
public:
    AmericanCallOption(double expiry, double strike);

    OptionType getOptionType() const override;
    double payoff(double spotPrice) const override;
};

#endif
