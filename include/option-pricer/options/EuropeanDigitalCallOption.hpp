#ifndef EUROPEANDIGITALCALLOPTION_HPP
#define EUROPEANDIGITALCALLOPTION_HPP

#include "EuropeanDigitalOption.hpp"

class EuropeanDigitalCallOption : public EuropeanDigitalOption {
public:
    EuropeanDigitalCallOption(double expiry, double strike);

    OptionType getOptionType() const override;
    double payoff(double asset_price) const override;
};

#endif
