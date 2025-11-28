#ifndef AMERICANPUTOPTION_H
#define AMERICANPUTOPTION_H

#include "AmericanOption.h"

class AmericanPutOption : public AmericanOption {
public:
    AmericanPutOption(double expiry, double strike);

    OptionType getOptionType() const override;
    double payoff(double spotPrice) const override;
};

#endif
