#ifndef AMERICANCALLOPTION_HPP
#define AMERICANCALLOPTION_HPP

#include "AmericanOption.hpp"

class AmericanCallOption : public AmericanOption {
public:
    AmericanCallOption(double expiry, double strike);

    OptionType getOptionType() const override;
    double payoff(double spotPrice) const override;
};

#endif
