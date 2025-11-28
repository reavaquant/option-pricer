#ifndef AMERICANPUTOPTION_HPP
#define AMERICANPUTOPTION_HPP

#include "AmericanOption.hpp"

class AmericanPutOption : public AmericanOption {
public:
    AmericanPutOption(double expiry, double strike);

    OptionType getOptionType() const override;
    double payoff(double spotPrice) const override;
};

#endif
